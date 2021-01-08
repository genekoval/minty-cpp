#include "core.test.h"

class CoreCreatorTest : public CoreTest {};

TEST_F(CoreCreatorTest, Add) {
    constexpr auto name = "   Test Creator   ";
    constexpr auto trimmed = "Test Creator";

    const auto id = api.add_creator(name);
    const auto creator = api.get_creator(id);

    ASSERT_EQ(trimmed, creator.name);
}
