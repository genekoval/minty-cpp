#include "db.test.h"

#include <sstream>

constexpr auto connection_string = "postgresql://minty@localhost/minty";

DatabaseTest::DatabaseTest() :
    connection(connection_string),
    database(connection_string)
{}

auto DatabaseTest::SetUp() ->  void { truncate_tables(); }

auto DatabaseTest::TearDown() ->  void { truncate_tables(); }

auto DatabaseTest::tables() -> std::vector<std::string> { return {}; }

auto DatabaseTest::truncate(std::string_view table) -> void {
    auto os = std::ostringstream();
    os << "TRUNCATE " << table << " CASCADE";

    auto tx = pqxx::nontransaction(connection);
    tx.exec(os.str());
}

auto DatabaseTest::truncate_tables() -> void {
    for (const auto& table : tables()) truncate(table);
}
