#include "database.test.hpp"

TEST_F(DatabasePostTest, CreatePost) {
    run([&]() -> ext::task<> {
        const auto id = co_await create_post();
        const auto post = co_await db->read_post(id);

        EXPECT_EQ(id, post.id);
        EXPECT_FALSE(post.title);
        EXPECT_FALSE(post.description);
        EXPECT_EQ(post.date_created, post.date_modified);
    }());
}

TEST_F(DatabasePostTest, CreatePostWithTitle) {
    constexpr auto title = "Post Title";

    run([&]() -> ext::task<> {
        const auto id = (co_await db->create_post(title, "", {}, {})).id;
        const auto post = co_await db->read_post(id);

        EXPECT_TRUE(post.title);
        EXPECT_EQ(title, post.title.value());
    }());
}

TEST_F(DatabasePostTest, CreatePostWithDescription) {
    constexpr auto description = "This is a test description.";

    run([&]() -> ext::task<> {
        const auto id = (co_await db->create_post("", description, {}, {})).id;
        const auto post = co_await db->read_post(id);

        EXPECT_TRUE(post.description);
        EXPECT_EQ(description, post.description.value());
    }());
}

TEST_F(DatabasePostObjectTest, CreatePostWithObjects) {
    run([&]() -> ext::task<> {
        for (const auto& object : objects) {
            co_await db->create_object(object, {}, {});
        }

        const auto id = (co_await db->create_post("", "", objects, {})).id;
        const auto result = co_await db->read_post_objects(id);

        EXPECT_EQ(3, result.size());

        for (auto i = 0u; i < result.size(); i++) {
            EXPECT_EQ(objects[i], result[i].id);
        }

        const auto sequence_objects = co_await with_sequence();

        for (auto i = 1u; i <= sequence_objects.size(); ++i) {
            EXPECT_EQ(i, sequence_objects[i - 1].sequence);
        }
    }());
}

TEST_F(DatabasePostTagTest, CreatePostWithTags) {
    constexpr auto names = std::array<std::string_view, 3> {
        "banana",
        "citrus",
        "apple"
    };

    run([&]() -> ext::task<> {
        auto tags = std::vector<UUID::uuid>();

        for (const auto& name : names) {
            tags.push_back(co_await db->create_tag(name));
        }

        const auto post = (co_await db->create_post("", "", {}, tags)).id;
        const auto post_tags = co_await db->read_post_tags(post);

        EXPECT_EQ(3, post_tags.size());
        EXPECT_EQ("apple", post_tags[0].name);
        EXPECT_EQ("banana", post_tags[1].name);
        EXPECT_EQ("citrus", post_tags[2].name);
    }());
}
