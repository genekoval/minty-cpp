#pragma once

#include "database.hpp"

namespace minty::core::db {
    class test_database : public database {
        std::shared_ptr<core::db::connection> connection;
    public:
        test_database(std::shared_ptr<core::db::connection>&& connection);

        auto connect() -> ext::task<connection_wrapper> override;
    };
}
