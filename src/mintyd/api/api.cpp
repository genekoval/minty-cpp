#include "api.h"

namespace minty::cli {
    api_container::api_container(const conf::settings& settings) :
        db(settings.database.connection.str()),
        object_store(settings.fstore.connection),
        bucket(
            object_store,
            object_store.fetch_bucket(settings.fstore.bucket).id
        ),
        objects(bucket),
        downloader(
            settings.downloader.host,
            settings.downloader.port
        ),
        search(settings.search.host.path),
        _api(db, objects, downloader, search)
    {}

    auto api_container::api() -> core::api& {
        return _api;
    }

    auto api_container::bucket_id() -> std::string_view {
        return bucket.id;
    }
}
