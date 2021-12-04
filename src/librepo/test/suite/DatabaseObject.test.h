#pragma once

#include "Database.test.h"

class DatabaseObjectTest : public DatabaseTest {
protected:
    static constexpr auto object_id = "0db27a78-b62f-41ff-9f6c-8e029b2a6342";
    static constexpr auto preview_id = "033d3aac-d27e-473c-b553-2d6b7aab9e88";
    static constexpr auto source_url = "https://example.com/hello.jpg";

    auto create_site() -> minty::repo::db::site;

    virtual auto tables() -> std::vector<std::string> override;
};
