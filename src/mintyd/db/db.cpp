#include "db.h"

#include <algorithm>
#include <ext/unix.h>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace {
    const auto sql_directory = fs::path(SQLDIR);
    const auto schema_file = (sql_directory / "minty.sql").string();
}

namespace minty::cli::data {
    client::client(const conf::settings& settings) :
        connection_string(settings.database.connection),
        program(settings.database.client)
    {}

    auto client::exec(const std::vector<std::string_view>& args) const -> void {
        auto arguments = std::vector<std::string_view> {
            "--dbname", connection_string
        };

        std::copy(args.begin(), args.end(), std::back_inserter(arguments));

        ext::exec(program, arguments);
    }

    auto client::init() const -> void {
        exec({
            "--set", "ON_ERROR_STOP=1",
            "--command", "DROP SCHEMA IF EXISTS minty CASCADE",
            "--command", "CREATE SCHEMA minty",
            "--command", "SET search_path = minty",
            "--file", schema_file
        });
    }
}
