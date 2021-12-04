#pragma once

#include "Database.test.h"

class DatabasePostTest : public DatabaseTest {
protected:
    const std::vector<std::string> objects = {
        "5c87bd88-27ea-4e4b-9146-3676ec2fb173",
        "5f8cc35b-262f-483f-8970-68a6f11eeafb",
        "e4917461-360d-4c96-a9aa-256e76525b4e"
    };

    auto create_post() -> std::string;

    auto create_post_with_objects() -> std::string;

    virtual auto tables() -> std::vector<std::string> override;
};
