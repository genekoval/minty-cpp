#include "search.test.h"

TEST_F(SearchPostTest, DeletePost) {
    const auto& post = add_post();

    const auto exists = [&]() -> bool {
        return client.doc_exists(index, post.id).send();
    };

    ASSERT_TRUE(exists());

    search.delete_post(post.id);

    ASSERT_FALSE(exists());
}
