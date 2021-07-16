#include "db.test.h"

#include <minty/repo/db/model.h>

class DatabaseCommentTest : public DatabaseTest {
protected:
    auto create_post() -> std::string {
        return database.create_post("", "", {}, {}).id;
    }

    auto tables() -> std::vector<std::string> override {
        return {"post"};
    }
};

TEST_F(DatabaseCommentTest, CreateRoot) {
    constexpr auto text = "First post.";

    const auto post_id = create_post();
    const auto comment = database.create_comment(post_id, {}, text);

    ASSERT_EQ(post_id, comment.post_id);
    ASSERT_FALSE(comment.parent_id.has_value());
    ASSERT_EQ(0, comment.indent);
    ASSERT_EQ(text, comment.content);
}

TEST_F(DatabaseCommentTest, CreateChild) {
    constexpr auto text = "Child post.";

    const auto post_id = create_post();
    const auto root = database.create_comment(post_id, {}, "First post.");
    const auto child = database.create_comment(post_id, root.id, text);

    ASSERT_EQ(post_id, child.post_id);
    ASSERT_EQ(root.id, child.parent_id);
    ASSERT_EQ(0, root.indent);
    ASSERT_EQ(1, child.indent);
    ASSERT_EQ(text, child.content);
}

TEST_F(DatabaseCommentTest, ReadNone) {
    const auto post_id = create_post();
    const auto comments = database.read_comments(post_id);

    ASSERT_TRUE(comments.empty());
}

TEST_F(DatabaseCommentTest, ReadOne) {
    constexpr auto text = "First post.";

    const auto post_id = create_post();
    const auto comment = database.create_comment(post_id, {}, text);

    auto comments = database.read_comments(post_id);

    ASSERT_EQ(1, comments.size());
    ASSERT_EQ(comment, comments.front());
}

TEST_F(DatabaseCommentTest, ReadNested) {
    const auto post_id = create_post();

    const auto root1 = database.create_comment(post_id, {}, "Root 1");
    const auto child1 = database.create_comment(post_id, root1.id, "Child 1");

    const auto root2 = database.create_comment(post_id, {}, "Root 2");
    const auto child2 = database.create_comment(post_id, root2.id, "Child 2");

    const auto comments = database.read_comments(post_id);

    ASSERT_EQ(4, comments.size());

    ASSERT_EQ(root2, comments[0]);
    ASSERT_EQ(root1, comments[1]);

    ASSERT_TRUE(comments[2] == child1 || comments[2] == child2);
    ASSERT_TRUE(comments[3] == child1 || comments[3] == child2);
}
