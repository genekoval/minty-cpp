#include "db.test.h"

#include <minty/repo/db/model.h>

class DatabasePostTest : public DatabaseTest {
protected:
    const std::optional<std::string_view> empty_opt;
    const std::vector<std::string> empty_vec;

    auto tables() -> std::vector<std::string> override {
        return {
            "creator",
            "object",
            "post",
            "tag"
        };
    }
};

TEST_F(DatabasePostTest, Create) {
    const auto id = database.create_post("", empty_vec, empty_opt, empty_vec);
    const auto post = database.read_post(id);

    ASSERT_EQ(id, post.id);
    ASSERT_FALSE(post.description.has_value());
    ASSERT_TRUE(post.objects.empty());
    ASSERT_TRUE(post.creators.empty());
    ASSERT_TRUE(post.tags.empty());
}

TEST_F(DatabasePostTest, CreateWithDescription) {
    constexpr auto description = "This is a test description.";

    const auto post = database.read_post(
        database.create_post(description, empty_vec, empty_opt, empty_vec)
    );

    ASSERT_TRUE(post.description.has_value());
    ASSERT_EQ(description, post.description.value());
}

TEST_F(DatabasePostTest, CreateWithObjects) {
    const auto objects = std::vector<std::string> {
        "7fdd7841-340d-4a78-b409-339b28c05313",
        "a63073ca-bec2-408f-8f93-f4d515f4792c",
        "1f777563-dd79-490c-8022-0c2e9fb430a5"
    };

    const auto id = database.create_post("", objects, empty_opt, empty_vec);
    const auto post = database.read_post(id);

    ASSERT_EQ(3, post.objects.size());

    for (auto i = 0u; i < post.objects.size(); i++) {
        ASSERT_EQ(objects[i], post.objects[i].id);
    }
}

TEST_F(DatabasePostTest, CreateWithCreator) {
    constexpr auto creator = "test";

    const auto post = database.read_post(database.create_post(
        "",
        empty_vec,
        database.create_creator(creator),
        empty_vec
    ));

    ASSERT_EQ(1, post.creators.size());
    ASSERT_EQ(creator, post.creators[0].name);
}

TEST_F(DatabasePostTest, CreateWithTags) {
    const auto names = std::array<std::string, 3> {
        "banana",
        "citrus",
        "apple"
    };

    auto tags = std::vector<std::string>();
    for (const auto& name : names) {
        tags.push_back(database.create_tag(name, "#000").id);
    }

    const auto post = database.read_post(database.create_post(
        "",
        empty_vec,
        empty_opt,
        tags
    ));

    ASSERT_EQ(3, post.tags.size());
    ASSERT_EQ("apple", post.tags[0].name);
    ASSERT_EQ("banana", post.tags[1].name);
    ASSERT_EQ("citrus", post.tags[2].name);
}
