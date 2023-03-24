#pragma once

#include "Database.test.hpp"

namespace minty::test {
    struct related_post {
        UUID::uuid post_id;
        UUID::uuid related;
    };
}

PGCPP_COMPOSITE_DECL(minty::test::related_post, "related_post");

class DatabasePostTest : public DatabaseTest {
protected:
    auto create_post() -> ext::task<UUID::uuid>;

    auto read_related_posts() ->
        ext::task<std::vector<minty::test::related_post>>;

    virtual auto tables() -> std::vector<std::string> override;
};
