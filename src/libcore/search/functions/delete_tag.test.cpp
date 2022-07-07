#include "search.test.h"

TEST_F(SearchTagTest, DeleteTag) {
    const auto& tag = add_tag();

    const auto exists = [&]() -> bool {
        return client.doc_exists(index, tag.id).send();
    };

    ASSERT_TRUE(exists());

    search.delete_tag(tag.id);

    ASSERT_FALSE(exists());
}
