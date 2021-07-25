#pragma once

#include <optional>
#include <string>
#include <vector>

namespace minty::repo::db {
    struct range {
        using index_type = int;

        index_type first;
        index_type last;
    };

    struct entity_base {
        auto operator==(const entity_base&) const -> bool = default;
    };

    template <int ColumnCount, typename ...Entities>
    struct entity : entity_base {
        static constexpr auto column_count =
            (ColumnCount + ... + Entities::column_count);
    };

    struct site : entity<4> {
        std::string id;
        std::string scheme;
        std::string host;
        std::optional<std::string> icon;
    };

    struct source : entity<2, site> {
        std::string id;
        std::string resource;
        site website;
    };

    struct object : entity<2, source> {
        std::string id;
        std::optional<std::string> preview_id;
        std::optional<source> src;
    };

    struct object_preview : entity<2> {
        std::string id;
        std::optional<std::string> preview_id;
    };

    struct tag : entity<9> {
        std::string id;
        std::string name;
        std::vector<std::string> aliases;
        std::optional<std::string> description;
        std::optional<std::string> avatar;
        std::optional<std::string> banner;
        unsigned int post_count;
        std::string date_created;
    };

    struct tag_name : entity<2> {
        std::string name;
        std::vector<std::string> aliases;
    };

    struct tag_name_update : entity<1, tag_name> {
        tag_name names;
        std::optional<std::string> old_name;
    };

    struct tag_preview : entity<3> {
        std::string id;
        std::string name;
        std::optional<std::string> avatar;
    };

    struct tag_text : entity<2> {
        std::string id;
        std::vector<std::string> names;
    };

    struct comment : entity<6> {
        std::string id;
        std::string post_id;
        std::optional<std::string> parent_id;
        unsigned int indent;
        std::string content;
        std::string date_created;
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

    struct post_search : entity<6> {
        decltype(post::id) id;
        decltype(post::title) title;
        decltype(post::description) description;
        decltype(post::date_created) date_created;
        decltype(post::date_modified) date_modified;
        std::vector<std::string> tags;
    };

    struct post_update : entity<3> {
        std::string id;
        std::optional<std::string> new_data;
        std::string date_modified;
    };
}
