#pragma once

#include "Database.test.hpp"

class DatabaseSiteTest : public DatabaseTest {
protected:
    virtual auto tables() -> std::vector<std::string> override;
};
