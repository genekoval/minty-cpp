#include "db.test.h"

#include <minty/repo/db/model.h>

class DatabasePostTest : public DatabaseTest {
protected:
    auto create_post() -> std::string {
        return database.create_post("", {}, {}, {});
    }

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
    const auto id = create_post();
    const auto post = database.read_post(id);

    ASSERT_EQ(id, post.id);
    ASSERT_FALSE(post.description.has_value());
    ASSERT_TRUE(post.creators.empty());
    ASSERT_TRUE(post.tags.empty());
}

TEST_F(DatabasePostTest, CreateWithDescription) {
    constexpr auto description = "This is a test description.";

    const auto post = database.read_post(
        database.create_post(description, {}, {}, {})
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

    const auto id = database.create_post("", objects, {}, {});
    const auto result = database.read_objects(id);

    ASSERT_EQ(3, result.size());

    for (auto i = 0u; i < result.size(); i++) {
        ASSERT_EQ(objects[i], result[i].id);
    }
}

TEST_F(DatabasePostTest, CreateWithCreator) {
    constexpr auto creator = "test";

    const auto post = database.read_post(
        database.create_post("", {}, {database.create_creator(creator)}, {})
    );

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

    const auto post = database.read_post(
        database.create_post("", {}, {}, tags)
    );

    ASSERT_EQ(3, post.tags.size());
    ASSERT_EQ("apple", post.tags[0].name);
    ASSERT_EQ("banana", post.tags[1].name);
    ASSERT_EQ("citrus", post.tags[2].name);
}

TEST_F(DatabasePostTest, CreateComment) {
    constexpr auto content = "Hello.";

    const auto post = create_post();
    const auto comment = database.create_comment(post, {}, content);

    ASSERT_EQ(post, comment.post_id);
    ASSERT_EQ(content, comment.content);
    ASSERT_FALSE(comment.parent_id.has_value());
}

TEST_F(DatabasePostTest, CreateNestedComment) {
    constexpr auto content = "Nested comment.";

    const auto post = create_post();
    const auto root = database.create_comment(post, {}, "Top comment.");
    const auto comment = database.create_comment(post, root.id, content);

    ASSERT_EQ(post, comment.post_id);
    ASSERT_EQ(content, comment.content);
    ASSERT_TRUE(comment.parent_id.has_value());
    ASSERT_EQ(root.id, comment.parent_id);
}
