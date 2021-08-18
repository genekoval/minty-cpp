#pragma once

#include <minty/conf/settings.h>

#include <span>
#include <string>
#include <string_view>

namespace minty::cli::data {
    class client {
        std::string client_program;
        std::string connection_string;
        std::string dump_program;
        std::string restore_program;

        auto analyze() const -> void;

        auto exec(
            std::string_view program,
            const std::vector<std::string_view>& args
        ) const -> void;

        auto wait_exec(
            std::string_view program,
            const std::vector<std::string_view>& args
        ) const -> void;
    public:
        client(const conf::settings& settings);

        auto dump(std::optional<std::string_view> file) const -> void;

        auto exec(const std::vector<std::string_view>& args) const -> void;

        auto init() const -> void;

        auto migrate() const -> void;

        auto restore(std::string_view file) const -> void;
    };
}
