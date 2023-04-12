#include "repo.hpp"

namespace minty::cli {
    container::container(const minty::conf::settings& settings) :
        db_params(pg::parameters::parse(
            settings.database.connection.parameters
        )),
        database(db_params),
        objects(settings.fstore.connection),
        downloader(settings.downloader),
        search(
            settings.search.ns,
            settings.search.node,
            settings.search.auth
        ),
        repo(database, objects, downloader, search)
    {}

    auto container::init(const conf::settings& settings) -> ext::task<> {
        co_await objects.init(settings.fstore.bucket);
    }
}
