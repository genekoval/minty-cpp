#pragma once

#include <filesystem>
#include <timber/timber>

namespace minty::cli {
    namespace conf {
        struct log {
            timber::level level = timber::level::info;
        };

        struct server {
            std::string objects;
            std::string path;
        };
    }

    struct settings {
        static auto load(std::string_view text) -> settings;
        static auto load(const std::filesystem::path& path) -> settings;

        conf::log log;
        conf::server server;
    };
}
