#include "db.test.h"

#include <sstream>

constexpr auto connection_string = "postgresql://minty@localhost/minty";

DatabaseTest::DatabaseTest() :
    connection(connection_string),
    database(connection_string)
{}

auto DatabaseTest::SetUp() ->  void { truncate_tables(); }

auto DatabaseTest::TearDown() ->  void { truncate_tables(); }

auto DatabaseTest::count(std::string_view table) -> int {
    return exec1(fmt::format("SELECT count(*) FROM {}", table))[0].as<int>();
}

auto DatabaseTest::tables() -> std::vector<std::string> { return {}; }

auto DatabaseTest::truncate(std::string_view table) -> void {
    exec0("TRUNCATE {} cascade", table);
}

auto DatabaseTest::truncate_tables() -> void {
    for (const auto& table : tables()) truncate(table);
}
