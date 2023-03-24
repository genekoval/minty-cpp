#pragma once

#include "../model.test.hpp"
#include "DatabasePost.test.hpp"

class DatabasePostObjectTest : public DatabasePostTest {
protected:
    const UUID::uuid new_object = "dc6c5f6e-30a2-46ed-9bb5-721f55895520";
    const std::vector<UUID::uuid> objects = {
        "5c87bd88-27ea-4e4b-9146-3676ec2fb173",
        "5f8cc35b-262f-483f-8970-68a6f11eeafb",
        "e4917461-360d-4c96-a9aa-256e76525b4e"
    };
    UUID::uuid post_id;

    auto create_post_with_objects() -> ext::task<UUID::uuid>;

    auto insert_object(
        std::int16_t position
    ) -> ext::task<std::vector<minty::test::sequence_object>>;

    auto insert_objects(
        const std::vector<UUID::uuid>& objects,
        std::int16_t position
    ) -> ext::task<std::vector<minty::test::sequence_object>>;

    auto run(ext::task<>&& task) -> void;

    virtual auto tables() -> std::vector<std::string> override;

    auto with_sequence() ->
        ext::task<std::vector<minty::test::sequence_object>>;
};
