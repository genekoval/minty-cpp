#pragma once

#include <ext/unix.h>
#include <map>
#include <netcore/netcore>
#include <optional>
#include <string>
#include <timber/timber>

namespace minty::conf {
    struct settings {
        struct s_daemon {
            ext::group group;
            std::string pidfile;
            ext::user user;
        };

        struct s_database {
            struct s_connection {
                std::map<std::string, std::string> options;

                auto str() const -> std::string;
            };

            std::string client = "psql";
            s_connection connection;
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

        static auto load(std::string_view text) -> settings;

        static auto load_file(std::string_view path) -> settings;

        s_daemon daemon;
        s_database database;
        s_downloader downloader;
        s_fstore fstore;
        s_log log;
        s_search search;
        netcore::unix_socket server;

        auto encode() const -> std::string;
    };
}
