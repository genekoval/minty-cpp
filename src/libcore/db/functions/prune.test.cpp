#include "database.test.hpp"

TEST_F(DatabasePruneTest, PruneObject) {
    run([&]() -> ext::task<> {
        co_await db->create_object(object_id, {}, {});

        EXPECT_EQ(1, co_await count("object"));
        EXPECT_EQ(1, co_await count("object_ref"));

        EXPECT_EQ(0, co_await prune_objects());
        EXPECT_EQ(1, co_await count("object"));
        EXPECT_EQ(1, co_await count("object_ref"));

        co_await db->prune();

        EXPECT_EQ(1, co_await count("object_ref"));

        EXPECT_EQ(1, co_await prune_objects());
        EXPECT_EQ(0, co_await count("object_ref"));
    }());
}

TEST_F(DatabasePruneTest, PruneObjectPreview) {
    run([&]() -> ext::task<> {
        co_await db->create_object(object_id, preview_id, {});

        EXPECT_EQ(1, co_await count("object"));
        EXPECT_EQ(2, co_await count("object_ref"));

        co_await db->prune();

        EXPECT_EQ(0, co_await count("object"));
        EXPECT_EQ(2, co_await count("object_ref"));

        EXPECT_EQ(2, co_await prune_objects());
        EXPECT_EQ(0, co_await count("object_ref"));
    }());
}

TEST_F(DatabasePruneTest, PrunePostObject) {
    run([&]() -> ext::task<> {
        const auto site = co_await create_site();
        const auto source = (co_await db->create_source(site, "/prune")).id;

        co_await db->create_object(object_id, {}, source);

        const auto objects = std::vector<UUID::uuid> { object_id };
        const auto draft = co_await db->create_post_draft();
        co_await db->create_post_objects(draft.id, objects, std::nullopt);

        EXPECT_EQ(1, co_await count("object"));
        EXPECT_EQ(2, co_await count("object_ref")); // object and site icon
        EXPECT_EQ(1, co_await count("post"));
        EXPECT_EQ(1, co_await count("site"));
        EXPECT_EQ(1, co_await count("source"));

        co_await db->prune();

        // Prune has no effect.
        EXPECT_EQ(1, co_await count("object"));
        EXPECT_EQ(2, co_await count("object_ref"));
        EXPECT_EQ(1, co_await count("post"));
        EXPECT_EQ(1, co_await count("site"));
        EXPECT_EQ(1, co_await count("source"));

        co_await db->delete_post(draft.id);

        EXPECT_EQ(1, co_await count("object"));
        EXPECT_EQ(2, co_await count("object_ref"));
        EXPECT_EQ(0, co_await count("post"));
        EXPECT_EQ(1, co_await count("site"));
        EXPECT_EQ(1, co_await count("source"));

        co_await db->prune();

        EXPECT_EQ(0, co_await count("object"));
        EXPECT_EQ(2, co_await count("object_ref"));
        EXPECT_EQ(0, co_await count("site"));
        EXPECT_EQ(0, co_await count("source"));

        EXPECT_EQ(2, co_await prune_objects());
        EXPECT_EQ(0, co_await count("object_ref"));
    }());
}

TEST_F(DatabasePruneTest, PruneSiteIcon) {
    run([&]() -> ext::task<> {
        co_await create_site();

        EXPECT_EQ(1, co_await count("site"));
        EXPECT_EQ(1, co_await count("object_ref"));

        co_await db->prune();

        EXPECT_EQ(0, co_await count("site"));
        EXPECT_EQ(1, co_await count("object_ref"));

        EXPECT_EQ(1, co_await prune_objects());
        EXPECT_EQ(0, co_await count("object_ref"));
    }());
}
