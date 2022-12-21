#pragma once

#include "DatabasePost.test.hpp"

class DatabasePostTagTest : public DatabasePostTest {
protected:
    virtual auto tables() -> std::vector<std::string> override;
};
