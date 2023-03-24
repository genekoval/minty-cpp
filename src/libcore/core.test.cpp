#include "core.test.hpp"

CoreTest::CoreTest() :
    db(new minty::core::db::mock_connection()),
    database(std::shared_ptr<minty::core::db::mock_connection>(db)),
    api(database, objects, downloader, search)
{}
