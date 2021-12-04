#pragma once

#include "Database.test.h"

class DatabaseSiteTest : public DatabaseTest {
protected:
    virtual auto tables() -> std::vector<std::string> override;
};
