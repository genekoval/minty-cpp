#pragma once

#include "Database.test.h"

class DatabasePruneTest : public DatabaseTest {
protected:
    static constexpr auto icon_id = "c49a4ac7-8f80-4228-b3a0-de5b07661256";
    static constexpr auto object_id = "f557b508-0271-46bd-acaa-62db96f6e0ae";
    static constexpr auto preview_id = "4d1ee79b-de61-4af7-9121-0187ded4afeb";

    auto create_site() -> std::string;

    auto prune_objects(bool commit) -> int;

    virtual auto tables() -> std::vector<std::string> override;
};
