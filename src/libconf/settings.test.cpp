#include <internal/conf/settings.hpp>

#include <gtest/gtest.h>

using netcore::unix_socket;

class ConfSettingsTest : public testing::Test {};

TEST_F(ConfSettingsTest, Decode) {
    constexpr auto yaml = R"(
daemon:
    user: root
    group: root
    pidfile: /run/minty/minty.pid

database:
    connection:
        host: localhost
        user: minty
        dbname: minty
    sqldir: /usr/share/minty/db

downloader: 127.0.0.1:3000

fstore:
    bucket: minty
    connection: /run/fstore/fstore.sock
    proxy:
        port: 5000

search:
    node: https://localhost:9200
    auth: apikey
    namespace: minty-test

server:
    path: /run/minty/minty.sock
)";

    const auto settings = minty::conf::settings::load(yaml);

    const auto& server = std::get<unix_socket>(settings.server);

    ASSERT_EQ(0, settings.daemon.user.uid());
    ASSERT_EQ(0, settings.daemon.group.gid());
    ASSERT_EQ("/run/minty/minty.pid", settings.daemon.pidfile);
    ASSERT_EQ("/run/minty/minty.sock", server.path);
    ASSERT_EQ(
        "dbname=minty host=localhost user=minty",
        settings.database.connection.str()
    );
    ASSERT_EQ("127.0.0.1:3000", settings.downloader);
    ASSERT_EQ("minty", settings.fstore.bucket);
    ASSERT_EQ("/run/fstore/fstore.sock", settings.fstore.connection);
    ASSERT_EQ(5000, settings.fstore.proxy.port);
    ASSERT_EQ("https://localhost:9200", settings.search.node);
    ASSERT_EQ("apikey", settings.search.auth);
    ASSERT_EQ("minty-test", settings.search.ns);
}

TEST_F(ConfSettingsTest, Encode) {
    constexpr auto yaml =
R"(database:
  connection:
    dbname: minty
    host: localhost
    user: minty

downloader: 127.0.0.1:3000

fstore:
  bucket: world
  connection: /tmp/fstore.sock
)";

    const auto settings = minty::conf::settings {
        .database = {
            .connection = {
                .parameters = {
                    {"host", "localhost"},
                    {"user", "minty"},
                    {"dbname", "minty"}
                }
            }
        },
        .downloader = "127.0.0.1:3000",
        .fstore = {
            .bucket = "world",
            .connection = "/tmp/fstore.sock"
        }
    };

    ASSERT_EQ(yaml, settings.encode());
}
