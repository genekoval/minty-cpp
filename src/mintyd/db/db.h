#pragma once

#include <minty/conf/settings.h>

#include <span>
#include <string>
#include <string_view>

namespace minty::cli::data {
    class client {
        std::string connection_string;
        std::string program;

        auto wait_exec(const std::vector<std::string_view>& args) const -> void;
    public:
        client(const conf::settings& settings);

        auto exec(const std::vector<std::string_view>& args) const -> void;

        auto init() const -> void;

        auto migrate() const -> void;
    };
}
