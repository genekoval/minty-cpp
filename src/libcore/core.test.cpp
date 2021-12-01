#include "core.test.h"

#include <minty/conf/settings.test.h>

namespace {
    auto settings() -> const minty::conf::settings& {
        return minty::test::settings();
    }
}

CoreTest::CoreTest() :
    db(settings().database.connection.str()),
    object_store(settings().fstore.connection),
    bucket(object_store.fetch_bucket(settings().fstore.bucket).id, object_store),
    downloader(settings().downloader.host, settings().downloader.port),
    search(settings().search.host),
    api(db, bucket, downloader, search)
{}
