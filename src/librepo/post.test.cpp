#include "db.test.h"

#include <minty/repo/db/model.h>

class DatabasePostTest : public DatabaseTest {
protected:
    auto create_post() -> std::string {
        return database.create_post("", "", {}, {});
    }

    auto tables() -> std::vector<std::string> override {
        return {
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
}

TEST_F(DatabasePostTest, CreateWithTitle) {
    constexpr auto title = "Post Title";

    const auto post = database.read_post(
        database.create_post(title, "", {}, {})
    );

    ASSERT_TRUE(post.title.has_value());
    ASSERT_EQ(title, post.title);
}

TEST_F(DatabasePostTest, CreateWithDescription) {
    constexpr auto description = "This is a test description.";

    const auto post = database.read_post(
        database.create_post("", description, {}, {})
    );

    ASSERT_TRUE(post.description.has_value());
    ASSERT_EQ(description, post.description);
}

TEST_F(DatabasePostTest, CreateWithObjects) {
    const auto objects = std::vector<std::string> {
        "7fdd7841-340d-4a78-b409-339b28c05313",
        "a63073ca-bec2-408f-8f93-f4d515f4792c",
        "1f777563-dd79-490c-8022-0c2e9fb430a5"
    };

    for (const auto& object : objects) {
        database.create_object(object, {}, {});
    }

    const auto id = database.create_post("", "", objects, {});
    const auto result = database.read_post_objects(id);

    ASSERT_EQ(3, result.size());

    for (auto i = 0u; i < result.size(); i++) {
        ASSERT_EQ(objects[i], result[i].id);
    }
}

TEST_F(DatabasePostTest, CreateWithTags) {
    const auto names = std::array<std::string, 3> {
        "banana",
        "citrus",
        "apple"
    };

    auto tags = std::vector<std::string>();
    for (const auto& name : names) {
        tags.push_back(database.create_tag(name));
    }

    const auto post = database.create_post("", "", {}, tags);
    const auto post_tags = database.read_post_tags(post);

    ASSERT_EQ(3, post_tags.size());
    ASSERT_EQ("apple", post_tags[0].name);
    ASSERT_EQ("banana", post_tags[1].name);
    ASSERT_EQ("citrus", post_tags[2].name);
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

TEST_F(DatabasePostTest, DeletePost) {
    const auto post1 = create_post();
    const auto post2 = create_post();

    ASSERT_EQ(2, count("post"));

    database.delete_post(post1);

    ASSERT_EQ(1, count("post"));

    const auto remaining = database.read_post(post2);
    ASSERT_EQ(post2, remaining.id);
}

TEST_F(DatabasePostTest, DeletePostDeletesComments) {
    const auto post = create_post();
    const auto comment = database.create_comment(post, {}, "Comment.");

    auto comments = database.read_comments(post);
    ASSERT_EQ(1, comments.size());
    ASSERT_EQ(comment, comments.front());

    database.delete_post(post);

    comments = database.read_comments(post);
    ASSERT_TRUE(comments.empty());
}
