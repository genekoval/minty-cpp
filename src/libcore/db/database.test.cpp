#include <internal/core/db/test_database.hpp>

using ptr = std::shared_ptr<minty::core::db::connection>;

namespace minty::core::db {
    test_database::test_database(ptr&& connection) :
        connection(std::forward<ptr>(connection))
    {}

    auto test_database::connect() -> ext::task<connection_wrapper> {
        co_return connection_wrapper(ptr(connection));
    }
}
