#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty::core {
    class search_engine {
        enum class event : net::event_t {
            add_post,
            add_tags,
            add_tag_alias,
            create_indices,
            delete_indices,
            delete_post,
            delete_tag,
            delete_tag_alias,
            find_tags_by_name,
            update_tag_name,
            version
        };

        using client = net::client<event>;

        const std::string endpoint;
        const net::error_list errors;

        auto connect() -> client;

        auto version() -> std::string;
    public:
        search_engine(std::string_view endpoint);

        auto add_post(const post_search& post) -> void;

        auto add_tags(std::span<const tag_text> tags) -> void;

        auto add_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> void;

        auto create_indices() -> void;

        auto delete_indices() -> void;

        auto delete_post(std::string_view post_id) -> void;

        auto delete_tag(std::string_view tag_id) -> void;

        auto delete_tag_alias(
            std::string_view tag_id,
            std::string_view alias
        ) -> void;

        auto find_tags_by_name(
            std::string_view term
        ) -> std::vector<std::string>;

        auto update_tag_name(
            std::string_view tag_id,
            std::string_view old_name,
            std::string_view new_name
        ) -> void;
    };
}
