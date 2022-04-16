#pragma once

#include "Database.test.h"

class DatabasePostTest : public DatabaseTest {
protected:
    auto create_post() -> UUID::uuid;

    virtual auto tables() -> std::vector<std::string> override;
};
