#include <minty/conf/settings.h>

#include <gtest/gtest.h>

class ConfSettingsTest : public testing::Test {};

TEST_F(ConfSettingsTest, Decode) {
    constexpr auto yaml = R"(
connection: /run/minty/minty.sock

database:
    connection:
        host: localhost
        user: minty
        dbname: minty

fstore:
    bucket: minty
    connection: /run/fstore/fstore.sock
)";

    const auto settings = minty::conf::settings::load(yaml);

    ASSERT_EQ("/run/minty/minty.sock", settings.connection);
    ASSERT_EQ(
        "host=localhost user=minty dbname=minty",
        settings.database.connection
    );
    ASSERT_EQ("minty", settings.fstore.bucket);
    ASSERT_EQ("/run/fstore/fstore.sock", settings.fstore.connection);
}

TEST_F(ConfSettingsTest, Encode) {
    constexpr auto yaml =
R"(connection: localhost:1234

database:
  connection: postgres://minty@localhost/minty

fstore:
  bucket: world
  connection: /tmp/fstore.sock
)";

    const auto settings = minty::conf::settings {
        .connection = "localhost:1234",
        .database = {
            .connection = "postgres://minty@localhost/minty"
        },
        .fstore = {
            .bucket = "world",
            .connection = "/tmp/fstore.sock"
        }
    };

    ASSERT_EQ(yaml, settings.encode());
}
