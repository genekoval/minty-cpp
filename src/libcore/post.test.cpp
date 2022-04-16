#include "core.test.h"

using minty::test::trimmed_text;
using minty::test::whitespace_text;
using testing::Eq;
using testing::Optional;
using testing::Return;

namespace {
    const auto post_search = minty::repo::db::post_search {
        .id = "d589757b-40a8-4bfa-a44a-502d690a8d2b"
    };
}

class CorePostTest : public CoreTest { };

TEST_F(CorePostTest, AddPostWhitespaceTitle) {
    EXPECT_CALL(db, create_post(
        std::string_view(trimmed_text),
        std::string_view(),
        std::vector<UUID::uuid>(),
        std::vector<std::string>()
    )).WillOnce(Return(post_search));

    EXPECT_CALL(search, add_post(post_search));

    api.add_post({ .title = whitespace_text });
}

TEST_F(CorePostTest, AddPostWhitespaceDescription) {
    EXPECT_CALL(db, create_post(
        std::string_view(),
        std::string_view(trimmed_text),
        std::vector<UUID::uuid>(),
        std::vector<std::string>()
    )).WillOnce(Return(post_search));

    EXPECT_CALL(search, add_post(post_search));

    api.add_post({ .description = whitespace_text });
}
