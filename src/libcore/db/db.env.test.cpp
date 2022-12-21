#include <internal/core/db/db.env.test.hpp>
#include <internal/conf/settings.env.test.hpp>

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

    auto DatabaseEnvironment::database() -> core::db::database& {
        static auto instance = core::db::database(connection_string(), 1);
        return instance;
    }

    auto DatabaseEnvironment::SetUp() -> void {
        connection();
        database();
    }
}
