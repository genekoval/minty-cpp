#include "core.test.h"

class CoreCommentTest : public CoreTest {
protected:
    const std::string post_id;

    CoreCommentTest() : post_id(api.add_post(
            "Test post",
            std::span<std::span<const std::byte>>(),
            {},
            {}
    )) {}

    auto add_root_comment(std::string_view content) -> minty::core::comment {
        return api.add_comment(post_id, {}, content);
    }

    auto add_child_comment(
        const minty::core::comment root,
        std::string_view content
    ) -> minty::core::comment {
        return api.add_comment(post_id, root.id, content);
    }
};

TEST_F(CoreCommentTest, AddRootComment) {
    constexpr auto content = "First comment.";

    const auto comment = add_root_comment(content);

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

    const auto c1 = add_root_comment("c1");
    const auto c2 = add_child_comment(c1, "c2");
    const auto c3 = add_child_comment(c2, "c3");
    const auto c4 = add_child_comment(c1, "c4");
    const auto c5 = add_root_comment("c5");
    const auto c6 = add_child_comment(c5, "c6");
    const auto c7 = add_root_comment("c7");

    auto comments = api.get_comments(post_id);
    const auto roots = comments.roots();

    ASSERT_EQ(3, roots.size());

    ASSERT_EQ(c7.id, roots[0]->model.id);
    ASSERT_TRUE(roots[0]->children.empty());

    ASSERT_EQ(c5.id, roots[1]->model.id);
    ASSERT_EQ(1, roots[1]->children.size());
    ASSERT_EQ(c6.id, roots[1]->children[0]->model.id);
    ASSERT_TRUE(roots[1]->children[0]->children.empty());

    ASSERT_EQ(c1.id, roots[2]->model.id);
    ASSERT_EQ(2, roots[2]->children.size());
    ASSERT_EQ(c4.id, roots[2]->children[0]->model.id);
    ASSERT_TRUE(roots[2]->children[0]->children.empty());
    ASSERT_EQ(c2.id, roots[2]->children[1]->model.id);
    ASSERT_EQ(1, roots[2]->children[1]->children.size());
    ASSERT_EQ(c3.id, roots[2]->children[1]->children[0]->model.id);
    ASSERT_TRUE(roots[2]->children[1]->children[0]->children.empty());
}
