#include <internal/conf/settings.env.test.hpp>
#include <minty/repo/db/db.env.test.h>

namespace {
    auto connection_string() -> const std::string& {
        static auto instance = minty::test::SettingsEnvironment::settings()
            .database
            .connection
            .str();

        return instance;
    }
}

namespace minty::test {
    auto DatabaseEnvironment::connection() -> pqxx::connection& {
        static auto instance = pqxx::connection(connection_string());
        return instance;
    }

    auto DatabaseEnvironment::database() -> repo::db::database& {
        static auto instance = repo::db::database(connection_string(), 1);
        return instance;
    }

    auto DatabaseEnvironment::SetUp() -> void {
        connection();
        database();
    }
}
