#pragma once

#include <conftools/yaml.h>

namespace minty::conf {
    struct settings {
        struct s_daemon {
            ext::group group;
            ext::user user;
        };

        struct s_database {
            std::optional<std::string> client;
            conftools::database_connection connection;
            std::optional<std::string> dump;
            std::optional<std::string> restore;
            std::string sqldir;
        };

        struct s_fstore {
            std::string bucket;
            std::string connection;
            std::string proxy;
        };

        struct s_http {
            std::vector<conftools::endpoint> listen;
            std::string cert;
            std::string key;
        };

        struct s_log {
            timber::level level = timber::level::info;
        };

        struct s_search {
            std::string node;
            std::string auth;
            std::string ns;
        };

        static auto load(std::string_view text) -> settings;

        static auto load_file(std::string_view path) -> settings;

        s_daemon daemon;
        s_database database;
        s_fstore fstore;
        s_http http;
        s_log log;
        s_search search;
        std::vector<conftools::endpoint> server;
    };

    auto initialize(std::string_view path) -> settings;
}
