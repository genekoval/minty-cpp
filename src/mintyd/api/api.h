#pragma once

#include <minty/conf/settings.h>
#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/search.h>
#include <minty/repo/db/database.h>

#include <fstore/client.h>

namespace minty::cli {
    class api_container {
        repo::db::database database;
        fstore::object_store object_store;
        std::string _bucket_id;
        fstore::bucket bucket;
        core::downloader downloader;
        core::search_engine search;
        core::api _api;
    public:
        api_container(const conf::settings& settings);

        auto api() -> core::api&;

        auto bucket_id() -> std::string_view;
    };
}
