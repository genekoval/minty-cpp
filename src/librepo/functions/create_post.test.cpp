#include "database.test.h"

TEST_F(DatabasePostTest, CreatePost) {
    const auto id = create_post();
    const auto post = database.read_post(id);

    ASSERT_EQ(id, post.id);
    ASSERT_FALSE(post.title);
    ASSERT_FALSE(post.description);
    ASSERT_EQ(post.date_created, post.date_modified);
}

TEST_F(DatabasePostTest, CreatePostWithTitle) {
    constexpr auto title = "Post Title";

    const auto id = database.create_post(title, "", {}, {}).id;
    const auto post = database.read_post(id);

    ASSERT_TRUE(post.title);
    ASSERT_EQ(title, post.title.value());
}

TEST_F(DatabasePostTest, CreatePostWithDescription) {
    constexpr auto description = "This is a test description.";

    const auto id = database.create_post("", description, {}, {}).id;
    const auto post = database.read_post(id);

    ASSERT_TRUE(post.description);
    ASSERT_EQ(description, post.description.value());
}

TEST_F(DatabasePostObjectTest, CreatePostWithObjects) {
    for (const auto& object : objects) {
        database.create_object(object, {}, {});
    }

    const auto id = database.create_post("", "", objects, {}).id;
    const auto result = database.read_post_objects(id);

    ASSERT_EQ(3, result.size());

    for (auto i = 0u; i < result.size(); i++) {
        ASSERT_EQ(objects[i], result[i].id);
    }

    const auto sequence_objects = with_sequence();

    for (auto i = 1u; i <= sequence_objects.size(); ++i) {
        ASSERT_EQ(i, sequence_objects[i - 1].sequence);
    }
}

TEST_F(DatabasePostTagTest, CreatePostWithTags) {
    const auto names = std::array<std::string, 3> {
        "banana",
        "citrus",
        "apple"
    };

    auto tags = std::vector<std::string>();
    for (const auto& name : names) {
        tags.push_back(database.create_tag(name));
    }

    const auto post = database.create_post("", "", {}, tags).id;
    const auto post_tags = database.read_post_tags(post);

    ASSERT_EQ(3, post_tags.size());
    ASSERT_EQ("apple", post_tags[0].name);
    ASSERT_EQ("banana", post_tags[1].name);
    ASSERT_EQ("citrus", post_tags[2].name);
}
