#pragma once

#include <optional>
#include <string>
#include <vector>

namespace minty::repo::db {
    template <int ColumnCount, typename ...Entities>
    struct entity {
        static constexpr auto column_count =
            (ColumnCount + ... + Entities::column_count);
    };

    struct site : entity<4> {
        std::string id;
        std::string name;
        std::string homepage;
        std::optional<std::string> thumbnail_id;
    };

    struct source : entity<2, site> {
        std::string id;
        std::string url;
        site website;
    };

    struct object : entity<2, source> {
        std::string id;
        std::optional<std::string> preview_id;
        std::optional<source> src;
    };

    struct tag : entity<4> {
        std::string id;
        std::string name;
        std::string color;
        std::string date_created;
    };

    struct creator : entity<8> {
        std::string id;
        std::string name;
        std::vector<std::string> aliases;
        std::optional<std::string> bio;
        std::optional<std::string> avatar;
        std::optional<std::string> banner;
        std::vector<source> sources;
        std::string date_added;
    };

    struct creator_preview : entity<3> {
        std::string id;
        std::string name;
        std::optional<std::string> avatar;
    };

    struct comment : entity<6> {
        std::string id;
        std::string post_id;
        std::optional<std::string> parent_id;
        std::string parent_path;
        std::string content;
        std::string date_created;
    };

    struct post : entity<7> {
        std::string id;
        std::optional<std::string> description;
        std::string date_created;
        std::string date_modified;
        std::vector<object> objects;
        std::vector<tag> tags;
        std::vector<creator_preview> creators;
    };
}
