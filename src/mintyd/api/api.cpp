#include "api.h"

#include <minty/core/downloader.h>
#include <minty/core/search/search.h>
#include <minty/repo/db/database.h>

#include <fstore/client.h>

namespace {
    class api_container {
        minty::repo::db::database db;
        minty::core::object_store objects;
        minty::core::downloader downloader;
        minty::core::search_engine search;
    public:
        minty::core::api api;

        api_container(
            const minty::conf::settings& settings
        ) :
            db(
                settings.database.connection.str(),
                settings.database.connections
            ),
            objects(settings.fstore.connection),
            downloader(
                settings.downloader.host,
                settings.downloader.port
            ),
            search(
                settings.search.ns,
                settings.search.node,
                settings.search.auth
            ),
            api(db, objects, downloader, search)
        {}

        auto init(const minty::conf::settings& settings) -> ext::task<> {
            co_await objects.init(settings.fstore.bucket);
        }
    };
}

namespace minty::cli {
    auto api(
        const conf::settings& settings,
        std::function<ext::task<>(core::api&)>&& action
    ) -> void {
        const auto task = [
            &settings,
            action = std::move(action)
        ]() -> ext::task<> {
            auto container = api_container(settings);
            co_await container.init(settings);
            co_await action(container.api);
        };

        netcore::async(task());
    }
}
