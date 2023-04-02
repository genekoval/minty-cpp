#include "database.test.hpp"

#include <minty/except.hpp>

TEST_F(DatabasePostTest, CreatePost) {
    constexpr auto title = "Create Post";

    run([&]() -> ext::task<> {
        const auto draft = co_await db->create_post_draft();
        const auto& id = draft.id;

        const auto update = co_await db->update_post_title(id, title);
        const auto published = co_await db->create_post(id);

        const auto post = co_await db->read_post(id);

        EXPECT_EQ(id, post.id);

        EXPECT_EQ(title, post.title.value());
        EXPECT_FALSE(post.description);

        EXPECT_EQ(minty::visibility::pub, post.visibility);

        EXPECT_EQ(post.date_created, post.date_modified);
        EXPECT_EQ(published, post.date_created);
        EXPECT_TRUE(draft.created < post.date_created);
        EXPECT_TRUE(update.date_modified < post.date_modified);
    }());
}

TEST_F(DatabasePostTest, CreatePostInvalid) {
    run([&]() -> ext::task<> {
        const auto id = co_await create_draft();
        co_await db->create_post(id);

        EXPECT_THROW(co_await db->create_post(id), minty::not_found);
    }());
}

TEST_F(DatabasePostTest, CreatePostNotFound) {
    run([&]() -> ext::task<> {
        EXPECT_THROW(co_await db->create_post(UUID::null()), minty::not_found);
    }());
}
