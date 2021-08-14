#include "db.test.h"

class DatabasePruneTest : public DatabaseTest {
protected:
    static constexpr auto icon_id = "c49a4ac7-8f80-4228-b3a0-de5b07661256";
    static constexpr auto object_id = "f557b508-0271-46bd-acaa-62db96f6e0ae";
    static constexpr auto preview_id = "4d1ee79b-de61-4af7-9121-0187ded4afeb";

    auto create_site() -> std::string {
        return database.create_site(
            "https",
            "example.com",
            icon_id
        ).id;
    }

    auto prune_objects(bool commit) -> int {
        auto pruned = 0;
        database.prune_objects([&](auto objects) -> bool {
            pruned = objects.size();
            return commit;
        });

        return pruned;
    }

    auto tables() -> std::vector<std::string> override {
        return {
            "object",
            "object_ref",
            "post",
            "site",
            "source"
        };
    }
};

TEST_F(DatabasePruneTest, CancelObjectPrune) {
    database.create_object(object_id, {}, {});
    database.prune();

    ASSERT_EQ(1, prune_objects(false));
    ASSERT_EQ(1, count("object_ref"));

    ASSERT_EQ(1, prune_objects(true));
    ASSERT_EQ(0, count("object_ref"));
}

TEST_F(DatabasePruneTest, PruneObject) {
    database.create_object(object_id, {}, {});

    ASSERT_EQ(1, count("object"));
    ASSERT_EQ(1, count("object_ref"));

    ASSERT_EQ(0, prune_objects(true));
    ASSERT_EQ(1, count("object"));
    ASSERT_EQ(1, count("object_ref"));

    database.prune();

    ASSERT_EQ(1, count("object_ref"));

    ASSERT_EQ(1, prune_objects(true));
    ASSERT_EQ(0, count("object_ref"));
}

TEST_F(DatabasePruneTest, PruneObjectPreview) {
    database.create_object(object_id, preview_id, {});

    ASSERT_EQ(1, count("object"));
    ASSERT_EQ(2, count("object_ref"));

    database.prune();

    ASSERT_EQ(0, count("object"));
    ASSERT_EQ(2, count("object_ref"));

    ASSERT_EQ(2, prune_objects(true));
    ASSERT_EQ(0, count("object_ref"));
}

TEST_F(DatabasePruneTest, PrunePostObject) {
    const auto site = create_site();
    const auto source = database.create_source(site, "/prune").id;
    database.create_object(object_id, {}, source);
    const auto objects = std::vector<std::string> { object_id };
    const auto post = database.create_post({}, {}, objects, {}).id;

    ASSERT_EQ(1, count("object"));
    ASSERT_EQ(2, count("object_ref")); // object and site icon
    ASSERT_EQ(1, count("post"));
    ASSERT_EQ(1, count("site"));
    ASSERT_EQ(1, count("source"));

    database.prune();

    // Prune has no effect.
    ASSERT_EQ(1, count("object"));
    ASSERT_EQ(2, count("object_ref"));
    ASSERT_EQ(1, count("post"));
    ASSERT_EQ(1, count("site"));
    ASSERT_EQ(1, count("source"));

    database.delete_post(post);

    ASSERT_EQ(1, count("object"));
    ASSERT_EQ(2, count("object_ref"));
    ASSERT_EQ(0, count("post"));
    ASSERT_EQ(1, count("site"));
    ASSERT_EQ(1, count("source"));

    database.prune();

    ASSERT_EQ(0, count("object"));
    ASSERT_EQ(2, count("object_ref"));
    ASSERT_EQ(0, count("site"));
    ASSERT_EQ(0, count("source"));

    ASSERT_EQ(2, prune_objects(true));
    ASSERT_EQ(0, count("object_ref"));
}

TEST_F(DatabasePruneTest, PruneSiteIcon) {
    const auto site = create_site();

    ASSERT_EQ(1, count("site"));
    ASSERT_EQ(1, count("object_ref"));

    database.prune();

    ASSERT_EQ(0, count("site"));
    ASSERT_EQ(1, count("object_ref"));

    ASSERT_EQ(1, prune_objects(true));
    ASSERT_EQ(0, count("object_ref"));
}
