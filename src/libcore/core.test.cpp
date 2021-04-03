#include "core.test.h"

constexpr auto db_connection = "postgresql://minty@localhost/minty";
constexpr auto fstore_connection = "/tmp/fstore.sock";
constexpr auto bucket_name = "minty";
constexpr auto harvest_host = "192.168.8.2";
constexpr auto harvest_port = "3000";
constexpr auto search_host = "/tmp/minty-search.sock";

CoreTest::CoreTest() :
    db(db_connection),
    object_store(fstore_connection),
    bucket(object_store.fetch_bucket(bucket_name).id, object_store),
    downloader(harvest_host, harvest_port),
    search(search_host),
    api(db, bucket, downloader, search)
{}
