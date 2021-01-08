#include "core.test.h"

constexpr auto db_connection = "postgresql://minty@localhost/minty";
constexpr auto fstore_connection = "/tmp/fstore.sock";
constexpr auto bucket_name = "minty";

CoreTest::CoreTest() :
    db(db_connection),
    object_store(fstore_connection),
    bucket(object_store.fetch_bucket(bucket_name).id, object_store),
    api(db, bucket)
{}
