#pragma once

#include "../model/model.test.h"
#include "DatabasePost.test.h"

class DatabasePostObjectTest : public DatabasePostTest {
protected:
    const std::string new_object = "dc6c5f6e-30a2-46ed-9bb5-721f55895520";
    std::string post;

    virtual auto SetUp() -> void override;

    auto insert_object(
        unsigned int position
    ) -> std::vector<minty::test::sequence_object>;

    auto insert_objects(
        const std::vector<std::string>& objects,
        unsigned int position
    ) -> std::vector<minty::test::sequence_object>;

    virtual auto tables() -> std::vector<std::string> override;

    auto with_sequence() -> std::vector<minty::test::sequence_object>;
};
