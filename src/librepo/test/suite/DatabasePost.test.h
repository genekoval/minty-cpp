#pragma once

#include "Database.test.h"

class DatabasePostTest : public DatabaseTest {
protected:
    auto create_post() -> std::string;

    virtual auto tables() -> std::vector<std::string> override;
};
