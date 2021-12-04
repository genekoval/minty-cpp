#pragma once

#include "DatabasePost.test.h"

class DatabasePostTagTest : public DatabasePostTest {
protected:
    virtual auto tables() -> std::vector<std::string> override;
};
