#include "api.h"

namespace minty::cli {
    api_container::api_container(const minty::conf::settings& settings) :
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

    auto api_container::init(const conf::settings& settings) -> ext::task<> {
        co_await objects.init(settings.fstore.bucket);
    }
}
