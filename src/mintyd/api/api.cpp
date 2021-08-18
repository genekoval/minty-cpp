#include "api.h"

namespace minty::cli {
    api_container::api_container(const conf::settings& settings) :
        database(settings.database.connection.str()),
        object_store(settings.fstore.connection),
        _bucket_id(object_store.fetch_bucket(settings.fstore.bucket).id),
        bucket(_bucket_id, object_store),
        downloader(
            settings.downloader.host,
            settings.downloader.port
        ),
        search(settings.search.host),
        _api(
            database,
            bucket,
            downloader,
            search
        )
    {}

    auto api_container::api() -> core::api& {
        return _api;
    }

    auto api_container::bucket_id() -> std::string_view {
        return _bucket_id;
    }
}
