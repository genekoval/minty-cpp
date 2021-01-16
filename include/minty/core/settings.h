#pragma once

#include <string>
#include <timber/timber>

namespace minty::core {
    struct settings {
        struct s_database {
            std::string connection;
        };

        struct s_fstore {
            std::string bucket;
            std::string connection;
        };

        struct s_log {
            timber::level level = timber::level::info;
        };

        static auto load(const std::string& text) -> settings;

        static auto load_file(const std::string& path) -> settings;

        std::string connection;
        s_database database;
        s_fstore fstore;
        s_log log;

        auto encode() const -> std::string;
    };
}
