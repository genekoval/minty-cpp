#include "db.test.h"

#include <sstream>

constexpr auto connection_string = "postgresql://minty@localhost/minty";

DatabaseTest::DatabaseTest() :
    connection(connection_string),
    db(connection_string)
{}

auto DatabaseTest::truncate(std::string_view table) -> void {
    auto os = std::ostringstream();
    os << "TRUNCATE " << table << " CASCADE";

    auto tx = pqxx::nontransaction(connection);
    tx.exec(os.str());
}
