#pragma once

#include <optional>
#include <string>
#include <vector>

namespace minty::repo::db {
    template <int ColumnCount, typename ...Entities>
    struct entity {
        static constexpr auto column_count =
            (ColumnCount + ... + Entities::column_count);

        auto operator==(const entity&) const -> bool = default;
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

    struct tag : entity<9> {
        std::string id;
        std::string name;
        std::vector<std::string> aliases;
        std::optional<std::string> description;
        std::optional<std::string> avatar;
        std::optional<std::string> banner;
        std::vector<source> sources;
        unsigned int post_count;
        std::string date_created;
    };

    struct tag_name : entity<2> {
        std::string name;
        std::vector<std::string> aliases;
    };

    struct tag_preview : entity<3> {
        std::string id;
        std::string name;
        std::optional<std::string> avatar;
    };

    struct comment : entity<6> {
        std::string id;
        std::string post_id;
        std::optional<std::string> parent_id;
        unsigned int indent;
        std::string content;
        std::string date_created;

        auto operator==(const comment&) const -> bool = default;
    };

    struct post : entity<7> {
        std::string id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        std::string date_created;
        std::string date_modified;
    };

    struct post_preview : entity<6> {
        std::string id;
        std::optional<std::string> title;
        std::optional<std::string> preview_id;
        unsigned int comment_count;
        unsigned int object_count;
        std::string date_created;
    };
}
