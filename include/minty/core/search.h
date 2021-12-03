#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>
#include <minty/test.h>

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty::core {
    class search_engine {
        enum class event : net::event_t {
            add_post,
            add_post_tag,
            add_posts,
            add_tags,
            add_tag_alias,
            create_indices,
            delete_indices,
            delete_post,
            delete_tag,
            delete_tag_alias,
            find_posts,
            find_tags,
            remove_post_tag,
            update_post_date_modified,
            update_post_description,
            update_post_title,
            update_tag_name,
            version
        };

        using client = net::client<event>;

        const std::string endpoint;
        const net::error_list errors;

        auto connect() -> client;

        auto version() -> std::string;
    public:
        search_engine() = default;

        search_engine(std::string_view endpoint);

        VIRTUAL_DESTRUCTOR(search_engine)

        VIRTUAL auto add_post(const post_search& post) -> void;

        VIRTUAL auto add_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        VIRTUAL auto add_posts(std::span<const post_search> posts) -> void;

        VIRTUAL auto add_tags(std::span<const tag_text> tags) -> void;

        VIRTUAL auto add_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> void;

        VIRTUAL auto create_indices() -> void;

        VIRTUAL auto delete_indices() -> void;

        VIRTUAL auto delete_post(std::string_view post_id) -> void;

        VIRTUAL auto delete_tag(std::string_view tag_id) -> void;

        VIRTUAL auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> void;

        VIRTUAL auto find_posts(
            const post_query& query
        ) -> search_result<std::string>;

        VIRTUAL auto find_tags(
            const tag_query& query
        ) -> search_result<std::string>;

        VIRTUAL auto remove_post_tag(
            std::string_view post_id,
            std::string_view tag_id
        ) -> void;

        VIRTUAL auto update_post_date_modified(
            std::string_view post_id,
            std::string_view date_modified
        ) -> void;

        VIRTUAL auto update_post_description(const post_update& post) -> void;

        VIRTUAL auto update_post_title(const post_update& post) -> void;

        VIRTUAL auto update_tag_name(
            std::string_view tag_id,
            std::string_view old_name,
            std::string_view new_name
        ) -> void;
    };
}
