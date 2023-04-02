#include "database.test.hpp"

TEST_F(DatabasePostTest, CreateDraft) {
    run([&]() -> ext::task<> {
        const auto draft = co_await db->create_post_draft();

        EXPECT_FALSE(draft.title);
        EXPECT_FALSE(draft.description);
        EXPECT_EQ(minty::visibility::draft, draft.visibility);
        EXPECT_EQ(draft.created, draft.modified);
    }());
}
