#pragma once

#include <optional>
#include <string>
#include <uuid++/uuid++>
#include <vector>

namespace minty::repo::db {
    struct range {
        using index_type = int;

        index_type first;
        index_type last;
    };

    struct site {
        std::string id;
        std::string scheme;
        std::string host;
        std::optional<UUID::uuid> icon;
    };

    struct source {
        std::string id;
        std::string resource;
        site website;
    };

    struct object {
        UUID::uuid id;
        std::optional<decltype(id)> preview_id;
        std::optional<source> src;
    };

    struct object_preview {
        decltype(object::id) id;
        decltype(object::preview_id) preview_id;
    };

    struct object_error {
        decltype(object::id) id;
        std::string message;
    };

    struct tag {
        UUID::uuid id;
        std::string name;
        std::vector<decltype(name)> aliases;
        std::optional<std::string> description;
        std::optional<decltype(object::id)> avatar;
        std::optional<decltype(object::id)> banner;
        unsigned int post_count;
        std::string date_created;
    };

    struct tag_name {
        decltype(tag::name) name;
        decltype(tag::aliases) aliases;
    };

    struct tag_name_update {
        tag_name names;
        std::optional<decltype(tag::name)> old_name;
    };

    struct tag_preview {
        decltype(tag::id) id;
        decltype(tag::name) name;
        decltype(tag::avatar) avatar;
    };

    struct tag_text {
        decltype(tag::id) id;
        std::vector<decltype(tag::name)> names;
    };

    struct post {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        std::string date_created;
        std::string date_modified;
    };

    struct comment {
        UUID::uuid id;
        decltype(post::id) post_id;
        std::optional<decltype(id)> parent_id;
        std::int16_t indent;
        std::string content;
        std::string date_created;

        auto operator==(const comment&) const -> bool = default;
    };

    struct post_preview {
        decltype(post::id) id;
        std::optional<std::string> title;
        std::optional<object_preview> preview;
        unsigned int comment_count;
        unsigned int object_count;
        std::string date_created;
    };

    struct post_search {
        decltype(post::id) id;
        decltype(post::title) title;
        decltype(post::description) description;
        decltype(post::date_created) date_created;
        decltype(post::date_modified) date_modified;
        std::vector<decltype(tag::id)> tags;

        auto operator==(const post_search&) const -> bool = default;
    };

    struct post_object_update {
        std::vector<object_preview> objects;
        std::string date_modified;
    };

    struct post_update {
        decltype(post::id) id;
        std::optional<std::string> new_data;
        std::string date_modified;
    };
}
