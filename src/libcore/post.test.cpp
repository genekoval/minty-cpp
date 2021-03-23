#include "core.test.h"

#include <fstream>

class CorePostTest : public CoreTest {
protected:
    static auto write_file(
        std::string_view name,
        std::string_view content
    ) -> std::filesystem::path {
        const auto path = std::filesystem::temp_directory_path() / name;

        auto file = std::ofstream(path);
        file << content << std::flush;

        return path;
    }

    auto add_post(minty::core::post_parts&& parts) -> minty::core::post {
        const auto id = api.add_post(parts);
        return api.get_post(id);
    }
};

TEST_F(CorePostTest, CreateEmptyTitle) {
    const auto post = add_post({.title = ""});
    ASSERT_FALSE(post.title.has_value());
}

TEST_F(CorePostTest, CreateEmptyDescription) {
    const auto post = add_post({.description = ""});
    ASSERT_FALSE(post.description.has_value());
}

TEST_F(CorePostTest, CreateWhitespaceTitle) {
    const auto post = add_post({.title = "   "});
    ASSERT_FALSE(post.title.has_value());
}

TEST_F(CorePostTest, CreateWhitespaceDescription) {
    const auto post = add_post({.description = "   "});
    ASSERT_FALSE(post.description.has_value());
}

TEST_F(CorePostTest, CreateFromBytes) {
    constexpr auto data = "Test data\n";
    constexpr auto size = 10;
    constexpr auto hash =
        "c0f5efbef0fe98aa90619444250b1a5e"
        "b23158d6686f0b190838f3d544ec85b9";

    auto object = api.add_object_data(size, [data, size](auto&& part) {
        part.write(std::span(
            reinterpret_cast<const std::byte*>(data),
            size
        ));
    });

    const auto post = add_post({
        .objects = {object}
    });

    ASSERT_EQ(1, post.objects.size());
    ASSERT_EQ(hash, post.objects[0].hash);
}

TEST_F(CorePostTest, CreateFromFiles) {
    constexpr auto one_data = "First file.\n";
    constexpr auto two_data = "Second file.\n";
    constexpr auto three_data = "Third file.\n";

    constexpr auto one_hash =
        "54681d8b997b55f6b65914c349813517"
        "9eb12255eadcffb3ca6386b2b2a525a4";
    constexpr auto two_hash =
        "b18f806b021da79fa5d84d5d895fc3f8"
        "aa418e30951f06194cb74f0593454505";
    constexpr auto three_hash =
        "552565f5ec09c7674b1756d1a268c64b"
        "18b23467fbc52ad0c206028a0c214437";

    auto files = std::vector<std::string>({
        write_file("one.txt", one_data),
        write_file("two.txt", two_data),
        write_file("three.txt", three_data)
    });

    auto objects = std::vector<std::string>();
    for (const auto& file : files) {
        objects.push_back(api.add_object_local(file));
    }

    const auto post = add_post({
        .objects = objects
    });

    ASSERT_EQ(3, post.objects.size());
    ASSERT_EQ(one_hash, post.objects[0].hash);
    ASSERT_EQ(two_hash, post.objects[1].hash);
    ASSERT_EQ(three_hash, post.objects[2].hash);
}
