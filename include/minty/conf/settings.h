#pragma once

#include <netcore/netcore>
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

        struct s_search {
            std::string host;
        };

        static auto load(const std::string& text) -> settings;

        static auto load_file(const std::string& path) -> settings;

        s_database database;
        s_downloader downloader;
        s_fstore fstore;
        s_log log;
        std::string pidfile;
        s_search search;
        netcore::unix_socket server;

        auto encode() const -> std::string;
    };
}
