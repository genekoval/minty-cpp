#include "db.test.h"

auto DatabaseTest::SetUp() ->  void { truncate_tables(); }

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
