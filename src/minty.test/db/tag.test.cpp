#include <gtest/gtest.h>
#include <sstream>
#include <pqxx/pqxx>

using namespace std::literals;

class DatabaseTagTest : public testing::Test {
protected:
    static constexpr auto connection_string =
        "postgresql://minty@localhost/minty";

    pqxx::connection db;
    pqxx::nontransaction tx;

    DatabaseTagTest() : db(connection_string), tx(db) {
        truncate("tag");
    }

    ~DatabaseTagTest() {
        truncate("tag");
    }

    auto truncate(std::string_view table) -> void {
        auto os = std::ostringstream();
        os << "TRUNCATE " << table << " CASCADE";

        tx.exec(os.str());
    }
};

TEST_F(DatabaseTagTest, CreateTag) {
    constexpr auto name = "mytag";
    constexpr auto color = "0f0f0f";

    auto row = tx.exec_params1(R"(
        SELECT name, color
        FROM create_tag($1, $2)
    )",
        name,
        color
    );

    ASSERT_EQ(name, row[0].as<std::string>());
    ASSERT_EQ(color, row[1].as<std::string>());

    row = tx.exec1(R"(
        SELECT name, color
        FROM tag
    )");

    ASSERT_EQ(name, row[0].as<std::string>());
    ASSERT_EQ(color, row[1].as<std::string>());
}
