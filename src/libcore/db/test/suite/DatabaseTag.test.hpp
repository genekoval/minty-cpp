#pragma once

#include "Database.test.hpp"

class DatabaseTagTest : public DatabaseTest {
protected:
    static constexpr auto tag_name = "minty";

    auto create_tag() -> ext::task<UUID::uuid>;

    virtual auto tables() -> std::vector<std::string> override;
};
