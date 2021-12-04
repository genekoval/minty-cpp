#include "database.test.h"

TEST_F(DatabaseTagTest, DeleteTagAlias) {
    constexpr auto alias = "Alias";

    const auto id = create_tag();

    database.create_tag_alias(id, alias);
    auto tag = database.read_tag(id);

    ASSERT_EQ(1, tag.aliases.size());

    database.delete_tag_alias(id, alias);
    tag = database.read_tag(id);

    ASSERT_TRUE(tag.aliases.empty());
}
