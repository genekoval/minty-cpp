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
        std::optional<UUID::uuid> preview_id;
        std::optional<source> src;
    };

    struct object_preview : entix::entity<2> {
        UUID::uuid id;
        std::optional<UUID::uuid> preview_id;
    };

    struct tag : entix::entity<9> {
        std::string id;
        std::string name;
        std::vector<std::string> aliases;
        std::optional<std::string> description;
        std::optional<std::string> avatar;
        std::optional<std::string> banner;
        unsigned int post_count;
        std::string date_created;
    };

    struct tag_name : entix::entity<2> {
        std::string name;
        std::vector<std::string> aliases;
    };

    struct tag_name_update : entix::entity<1, tag_name> {
        tag_name names;
        std::optional<std::string> old_name;
    };

    struct tag_preview : entix::entity<3> {
        std::string id;
        std::string name;
        std::optional<UUID::uuid> avatar;
    };

    struct tag_text : entix::entity<2> {
        std::string id;
        std::vector<std::string> names;
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
        std::vector<std::string> tags;

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
