#include "core.test.h"

using minty::test::trimmed_text;
using minty::test::whitespace_text;
using testing::Return;

class CoreTagTest : public CoreTest {};

TEST_F(CoreTagTest, AddTag) {
    constexpr auto tag_id = "1";

    EXPECT_CALL(db, create_tag(trimmed_text)).WillOnce(Return(tag_id));
    EXPECT_CALL(search, add_tag_alias(tag_id, trimmed_text));

    const auto result = api.add_tag(whitespace_text);

    ASSERT_EQ(tag_id, result);
}
