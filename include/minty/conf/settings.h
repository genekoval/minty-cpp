#pragma once

#include <optional>
#include <string>
#include <timber/timber>

namespace minty::conf {
    struct settings {
        struct s_database {
            std::string connection;
        };

        struct s_fstore {
            struct s_proxy {
                std::optional<std::string> host;
                unsigned short port;
            };

            std::string bucket;
            std::string connection;
            std::optional<s_proxy> proxy;
        };

        struct s_downloader {
            std::string host;
            std::string port;
        };

        struct s_log {
            timber::level level = timber::level::info;
        };

        static auto load(const std::string& text) -> settings;

        static auto load_file(const std::string& path) -> settings;

        std::string connection;
        s_database database;
        s_downloader downloader;
        s_fstore fstore;
        s_log log;

        auto encode() const -> std::string;
    };
}
