#pragma once

#include "Database.test.hpp"

class DatabasePostTest : public DatabaseTest {
protected:
    auto create_post() -> UUID::uuid;

    virtual auto tables() -> std::vector<std::string> override;
};
