#include "core.test.hpp"

using minty::test::trimmed_text;
using minty::test::whitespace_text;
using testing::Return;

namespace {
    const auto post_search = minty::core::db::post_search {
        .id = "d589757b-40a8-4bfa-a44a-502d690a8d2b"
    };
}

using CorePostTest = CoreTest;

TEST_F(CorePostTest, AddPostWhitespaceTitle) {
    EXPECT_CALL(db, create_post(
        std::string_view(trimmed_text),
        std::string_view(),
        std::vector<UUID::uuid>(),
        std::vector<UUID::uuid>()
    )).WillOnce(Return(post_search));

    EXPECT_CALL(search, add_post(post_search))
        .WillOnce(Return(ext::make_task()));

    netcore::run([&]() -> ext::task<> {
        co_await api.add_post({ .title = whitespace_text });
    }());
}

TEST_F(CorePostTest, AddPostWhitespaceDescription) {
    EXPECT_CALL(db, create_post(
        std::string_view(),
        std::string_view(trimmed_text),
        std::vector<UUID::uuid>(),
        std::vector<UUID::uuid>()
    )).WillOnce(Return(post_search));

    EXPECT_CALL(search, add_post(post_search))
        .WillOnce(Return(ext::make_task()));

    netcore::run([&]() -> ext::task<> {
        co_await api.add_post({ .description = whitespace_text });
    }());
}
