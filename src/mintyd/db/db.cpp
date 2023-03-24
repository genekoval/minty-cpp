#include "db.hpp"

namespace minty::cli {
    auto database(std::string_view confpath) -> dbtools::postgresql {
        const auto settings = conf::initialize(confpath);
        return database(settings);
    }

    auto database(const conf::settings& settings) -> dbtools::postgresql {
        const auto& config = settings.database;
        auto options = dbtools::postgresql::options();

        if (config.client) options.client_program = *config.client;
        options.connection_string = config.connection.str();
        if (config.dump) options.dump_program = *config.dump;
        if (config.restore) options.restore_program = *config.restore;

        options.sql_directory = settings.database.sqldir;

        return dbtools::postgresql(std::move(options));
    }
}
