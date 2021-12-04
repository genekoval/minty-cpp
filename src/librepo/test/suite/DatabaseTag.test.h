#pragma once

#include "Database.test.h"

class DatabaseTagTest : public DatabaseTest {
protected:
    static constexpr auto tag_name = "minty";

    auto create_tag() -> std::string;

    virtual auto tables() -> std::vector<std::string> override;
};
