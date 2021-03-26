#include "core.test.h"

class CoreTagTest : public CoreTest {};

TEST_F(CoreTagTest, Add) {
    constexpr auto name = "   Test Tag   ";
    constexpr auto trimmed = "Test Tag";

    const auto id = api.add_tag(name);
    const auto tag = api.get_tag(id);

    ASSERT_EQ(trimmed, tag.name);
}
