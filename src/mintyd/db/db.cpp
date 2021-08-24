#include "db.h"

#include <algorithm>
#include <ext/unix.h>
#include <fmt/format.h>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace {
    constexpr auto stop_on_error = "--set=ON_ERROR_STOP=1";

    const auto sql_directory = fs::path(SQLDIR);

    const auto api_schema = (sql_directory / "minty.sql").string();
    const auto data_schema = (sql_directory / "data.sql").string();
}

namespace minty::cli::data {
    client::client(const conf::settings& settings) :
        client_program(settings.database.client),
        connection_string(settings.database.connection.str()),
        dump_program(settings.database.dump),
        restore_program(settings.database.restore)
    {}

    auto client::analyze() const -> void {
        wait_exec(client_program, {"--command", "ANALYZE"});
    }

    auto client::dump(std::optional<std::string_view> file) const -> void {
        const auto target = file.value_or("minty.dump");

        wait_exec(dump_program, {
            "--format", "custom",
            "--file", target
        });

        INFO() << "Saved database dump to: " << target;
    }

    auto client::exec(
        std::string_view program,
        const std::vector<std::string_view>& args
    ) const -> void {
        auto arguments = std::vector<std::string_view> {
            "--dbname", connection_string
        };

        std::copy(args.begin(), args.end(), std::back_inserter(arguments));

        if (timber::reporting_level >= timber::level::debug) {
            auto os = std::ostringstream();
            os << "EXEC " << program;

            for (const auto& arg : arguments) {
                os << " ";

                if (arg.find(" ") != std::string_view::npos) {
                    os << "\"" << arg << "\"";
                }
                else os << arg;
            }

            DEBUG() << os.str();
        }

        ext::exec(program, arguments);
    }

    auto client::exec(const std::vector<std::string_view>& args) const -> void {
        exec(client_program, args);
    }

    auto client::init() const -> void {
        wait_exec(client_program, {
            stop_on_error,
            "--file", data_schema
        });

        migrate();
    }

    auto client::migrate() const -> void {
        wait_exec(client_program, {
            stop_on_error,
            "--file", api_schema
        });
    }

    auto client::restore(std::string_view file) const -> void {
        wait_exec(restore_program, {
            "--clean",
            "--if-exists",
            file
        });

        analyze();
    }

    auto client::wait_exec(
        std::string_view program,
        const std::vector<std::string_view>& args
    ) const -> void {
        const auto parent = ext::process::fork();

        if (!parent) exec(program, args);

        const auto process = *parent;
        const auto exit = process.wait();

        if (exit.code == CLD_EXITED) {
            if (exit.status == 0) return;
            throw std::runtime_error(fmt::format(
                "{} exited with code {}",
                program,
                exit.status
            ));
        }

        if (exit.code == CLD_KILLED || exit.code == CLD_DUMPED) {
            throw std::runtime_error(fmt::format(
                "{} was killed by signal {}",
                program,
                exit.status
            ));
        }

        throw std::runtime_error(fmt::format(
            "{} did not succeed",
            program
        ));
    }
}
