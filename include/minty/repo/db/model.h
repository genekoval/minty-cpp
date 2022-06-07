#pragma once

#include <entix/entity>
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

    struct site : entix::entity<4> {
        std::string id;
        std::string scheme;
        std::string host;
        std::optional<UUID::uuid> icon;
    };

    struct source : entix::entity<2, site> {
        std::string id;
        std::string resource;
        site website;
    };

    struct object : entix::entity<2, source> {
        UUID::uuid id;
        std::optional<decltype(id)> preview_id;
        std::optional<source> src;
    };

    struct object_preview : entix::entity<2> {
        decltype(object::id) id;
        decltype(object::preview_id) preview_id;
    };

    struct object_error : entix::entity<2> {
        decltype(object::id) id;
        std::string message;
    };

    struct tag : entix::entity<9> {
        UUID::uuid id;
        std::string name;
        std::vector<decltype(name)> aliases;
        std::optional<std::string> description;
        std::optional<decltype(object::id)> avatar;
        std::optional<decltype(object::id)> banner;
        unsigned int post_count;
        std::string date_created;
    };

    struct tag_name : entix::entity<2> {
        decltype(tag::name) name;
        decltype(tag::aliases) aliases;
    };

    struct tag_name_update : entix::entity<1, tag_name> {
        tag_name names;
        std::optional<decltype(tag::name)> old_name;
    };

    struct tag_preview : entix::entity<3> {
        decltype(tag::id) id;
        decltype(tag::name) name;
        decltype(tag::avatar) avatar;
    };

    struct tag_text : entix::entity<2> {
        decltype(tag::id) id;
        std::vector<decltype(tag::name)> names;
    };

    struct post : entix::entity<7> {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        std::string date_created;
        std::string date_modified;
    };

    struct comment : entix::entity<6> {
        UUID::uuid id;
        decltype(post::id) post_id;
        std::optional<decltype(id)> parent_id;
        std::int16_t indent;
        std::string content;
        std::string date_created;

        auto operator==(const comment&) const -> bool = default;
    };

    struct post_preview : entix::entity<5, object_preview> {
        decltype(post::id) id;
        std::optional<std::string> title;
        std::optional<object_preview> preview;
        unsigned int comment_count;
        unsigned int object_count;
        std::string date_created;
    };

    struct post_search : entix::entity<6> {
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

    struct post_update : entix::entity<3> {
        decltype(post::id) id;
        std::optional<std::string> new_data;
        std::string date_modified;
    };
}
