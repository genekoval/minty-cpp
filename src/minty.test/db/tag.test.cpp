#include <minty/repo/db/database.h>

#include <gtest/gtest.h>
#include <sstream>

using namespace std::literals;

class DatabaseTagTest : public testing::Test {
protected:
    static constexpr auto connection_string =
        "postgresql://minty@localhost/minty";

    minty::repo::db::database db;
    pqxx::connection con;
    pqxx::nontransaction tx;

    DatabaseTagTest() :
        db(connection_string),
        con(connection_string),
        tx(con)
    {
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

    const auto tag = db.create_tag(name, color);

    ASSERT_EQ(name, tag.name);
    ASSERT_EQ(color, tag.color);
}
