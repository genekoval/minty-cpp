#include <minty/conf/settings.h>

#include <gtest/gtest.h>

class ConfSettingsTest : public testing::Test {};

TEST_F(ConfSettingsTest, Decode) {
    constexpr auto yaml = R"(
database:
    connection:
        host: localhost
        user: minty
        dbname: minty

downloader:
    host: 127.0.0.1
    port: 3000

fstore:
    bucket: minty
    connection: /run/fstore/fstore.sock

search:
    host: /run/minty-search.sock

server:
    path: /run/minty/minty.sock
)";

    const auto settings = minty::conf::settings::load(yaml);

    ASSERT_EQ("/run/minty/minty.sock", settings.server.path);
    ASSERT_EQ(
        "host=localhost user=minty dbname=minty",
        settings.database.connection
    );
    ASSERT_EQ("127.0.0.1", settings.downloader.host);
    ASSERT_EQ("3000", settings.downloader.port);
    ASSERT_EQ("minty", settings.fstore.bucket);
    ASSERT_EQ("/run/fstore/fstore.sock", settings.fstore.connection);
    ASSERT_EQ("/run/minty-search.sock", settings.search.host);
}

TEST_F(ConfSettingsTest, Encode) {
    constexpr auto yaml =
R"(server:
  path: /run/minty/minty.sock

database:
  connection: postgres://minty@localhost/minty

downloader:
  host: 127.0.0.1
  port: 3000

fstore:
  bucket: world
  connection: /tmp/fstore.sock
)";

    const auto settings = minty::conf::settings {
        .database = {
            .connection = "postgres://minty@localhost/minty"
        },
        .downloader = {
            .host = "127.0.0.1",
            .port = "3000"
        },
        .fstore = {
            .bucket = "world",
            .connection = "/tmp/fstore.sock"
        },
        .server = {
            .path = "/run/minty/minty.sock"
        }
    };

    ASSERT_EQ(yaml, settings.encode());
}
