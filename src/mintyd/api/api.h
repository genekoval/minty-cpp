#pragma once

#include <minty/conf/settings.h>
#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/search/search.h>
#include <minty/repo/db/database.h>

#include <fstore/client.h>

namespace minty::cli {
    class api_container {
        repo::db::database db;
        fstore::object_store object_store;
        fstore::bucket bucket;
        core::object_store objects;
        core::downloader downloader;
        core::search_engine search;
        core::api _api;
    public:
        api_container(const conf::settings& settings);

        auto api() -> core::api&;

        auto bucket_id() -> std::string_view;
    };
}
