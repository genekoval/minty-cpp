#include "core.test.hpp"

#include <gmock/gmock.h>

using minty::comment;
using testing::_;
using testing::Return;

class CoreCommentTest : public CoreTest {
protected:
    const UUID::uuid post_id = "216311b1-9b62-4dab-9182-f286a9abe05b";
};

TEST_F(CoreCommentTest, AddRootComment) {
    constexpr auto content = "First comment.";

    EXPECT_CALL(*db, create_comment(
        post_id,
        content)
    ).WillOnce(Return(ext::make_task(comment { .content = content })));

    [&]() -> ext::detached_task {
        const auto comment = co_await api.add_comment(post_id, content);
        EXPECT_EQ(content, comment.content);
    }();
}

TEST_F(CoreCommentTest, ReadCommentChain) {
    const UUID::uuid id1 = "4264a598-2001-432c-8212-33ce1e19f18c";
    const UUID::uuid id2 = "7efc1390-dabb-4684-848e-41a8e7999d4b";
    const UUID::uuid id3 = "70fed141-fdba-47ab-8709-aa133920861f";
    const UUID::uuid id4 = "50589dd1-e2a2-4eb3-a7b4-a92bdb831f5d";
    const UUID::uuid id5 = "0e7303ec-f47c-4eab-92d1-3b1f6a046f8d";
    const UUID::uuid id6 = "baa4797d-41b8-47b7-94dd-e8f07cdfc51b";
    const UUID::uuid id7 = "c473cddb-5757-43f9-b6e6-fabb4116cb87";

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

    const auto c1 = comment { .id = id1 };
    const auto c2 = comment { .id = id2, .parent_id = id1 };
    const auto c3 = comment { .id = id3, .parent_id = id2 };
    const auto c4 = comment { .id = id4, .parent_id = id1 };
    const auto c5 = comment { .id = id5 };
    const auto c6 = comment { .id = id6, .parent_id = id5 };
    const auto c7 = comment { .id = id7 };

    EXPECT_CALL(*db, read_comments(_))
        .WillOnce(Return(ext::make_task(std::vector<comment> {
            c7,
            c5,
            c1,
            c4,
            c2,
            c6,
            c3
        })));

    auto comments = minty::comment_tree();

    [&]() -> ext::detached_task {
        comments = co_await api.get_comments(post_id);
    }();

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
