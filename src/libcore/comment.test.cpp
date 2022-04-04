#include "core.test.h"

#include <gmock/gmock.h>

using minty::repo::db::comment;
using testing::_;
using testing::Return;

class CoreCommentTest : public CoreTest {
protected:
    const std::string post_id = "1";
};

TEST_F(CoreCommentTest, AddRootComment) {
    constexpr auto content = "First comment.";

    EXPECT_CALL(db, create_comment(
        post_id,
        content)
    ).WillOnce(Return(minty::repo::db::comment { .content = content }));

    const auto comment = api.add_comment(post_id, content);

    ASSERT_EQ(content, comment.content);
}

TEST_F(CoreCommentTest, ReadCommentChain) {
    /*
        Assume the following comment structure:

        c1
            c2
                c3
            c4
        c5
            c6
        c7
    */

    const auto c1 = comment { .id = "1" };
    const auto c2 = comment { .id = "2", .parent_id = "1" };
    const auto c3 = comment { .id = "3", .parent_id = "2" };
    const auto c4 = comment { .id = "4", .parent_id = "1" };
    const auto c5 = comment { .id = "5" };
    const auto c6 = comment { .id = "6", .parent_id = "5" };
    const auto c7 = comment { .id = "7"};

    EXPECT_CALL(db, read_comments(_))
        .WillOnce(Return(std::vector<comment> {
            c7,
            c5,
            c1,
            c4,
            c2,
            c6,
            c3
        }));

    auto comments = api.get_comments(post_id);
    const auto& roots = comments.roots;

    ASSERT_EQ(3, roots.size());

    ASSERT_EQ(c7.id, roots[0]->data.id);
    ASSERT_TRUE(roots[0]->children.empty());

    ASSERT_EQ(c5.id, roots[1]->data.id);
    ASSERT_EQ(1, roots[1]->children.size());
    ASSERT_EQ(c6.id, roots[1]->children[0]->data.id);
    ASSERT_TRUE(roots[1]->children[0]->children.empty());

    ASSERT_EQ(c1.id, roots[2]->data.id);
    ASSERT_EQ(2, roots[2]->children.size());
    ASSERT_EQ(c4.id, roots[2]->children[0]->data.id);
    ASSERT_TRUE(roots[2]->children[0]->children.empty());
    ASSERT_EQ(c2.id, roots[2]->children[1]->data.id);
    ASSERT_EQ(1, roots[2]->children[1]->children.size());
    ASSERT_EQ(c3.id, roots[2]->children[1]->children[0]->data.id);
    ASSERT_TRUE(roots[2]->children[1]->children[0]->children.empty());
}
