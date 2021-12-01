#include "db.test.h"

#include <minty/conf/settings.test.h>

#include <sstream>

namespace {
    auto connection_string() -> std::string {
        return minty::test::settings().database.connection.str();
    }
}

DatabaseTest::DatabaseTest() :
    connection(connection_string()),
    database(connection_string())
{}

auto DatabaseTest::SetUp() ->  void { truncate_tables(); }

auto DatabaseTest::TearDown() ->  void { truncate_tables(); }

auto DatabaseTest::count(std::string_view table) -> int {
    return exec1(fmt::format("SELECT count(*) FROM data.{}", table))[0].as<int>();
}

auto DatabaseTest::tables() -> std::vector<std::string> { return {}; }

auto DatabaseTest::truncate(std::string_view table) -> void {
    exec0("TRUNCATE data.{} cascade", table);
}

auto DatabaseTest::truncate_tables() -> void {
    for (const auto& table : tables()) truncate(table);
}
