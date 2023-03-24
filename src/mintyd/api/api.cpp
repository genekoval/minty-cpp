#include "api.h"

namespace minty::cli {
    api_container::api_container(const minty::conf::settings& settings) :
        db_params(pg::parameters::parse(
            settings.database.connection.parameters
        )),
        database(db_params),
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
        api(database, objects, downloader, search)
    {}

    auto api_container::init(const conf::settings& settings) -> ext::task<> {
        co_await objects.init(settings.fstore.bucket);
    }
}
