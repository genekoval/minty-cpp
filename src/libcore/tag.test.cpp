#include "core.test.h"

using minty::test::trimmed_text;
using minty::test::whitespace_text;
using testing::Return;

class CoreTagTest : public CoreTest {};

TEST_F(CoreTagTest, AddTag) {
    const UUID::uuid tag_id = "1e2478f5-5a29-40b8-8c0e-9ef243f0deb5";

    EXPECT_CALL(db, create_tag(trimmed_text)).WillOnce(Return(tag_id));
    EXPECT_CALL(search, add_tag_alias(tag_id, trimmed_text))
        .WillOnce(Return(ext::make_task()));

    auto result = UUID::uuid();

    netcore::run([&]() -> ext::task<> {
        result = co_await api.add_tag(whitespace_text);
    }());

    ASSERT_EQ(tag_id, result);
}
