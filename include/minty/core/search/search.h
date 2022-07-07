#pragma once

#include "index.h"

#include <minty/core/model.h>
#include <minty/test.h>

#include <elasticsearch/elasticsearch>

namespace minty::core {
    class search_engine {
        friend class index;

        elastic::elasticsearch client;

        template <typename T>
        auto bulk_index(
            std::string_view index,
            std::span<const T> documents
        ) -> std::vector<std::string>;

        auto create_index(const index& index) -> void;

        auto delete_indices(
            std::initializer_list<std::string_view> index
        ) -> void;

        auto search(
            std::string_view index,
            std::string_view query
        ) -> elastic::json;

        auto search_ids(
            std::string_view index,
            std::string_view query
        ) -> search_result<UUID::uuid>;
    public:
        index post_index;
        index tag_index;

        search_engine() = default;

        search_engine(
            std::string_view ns,
            std::string_view host,
            std::string_view api_key
        );

        VIRTUAL_DESTRUCTOR(search_engine)

        VIRTUAL auto add_post(const post_search& post) -> void;

        VIRTUAL auto add_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> void;

        [[nodiscard]]
        VIRTUAL auto add_posts(
            std::span<const post_search> posts
        ) -> std::vector<std::string>;

        [[nodiscard]]
        VIRTUAL auto add_tags(
            std::span<const tag_text> tags
        ) -> std::vector<std::string>;

        VIRTUAL auto add_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> void;

        VIRTUAL auto create_indices() -> void;

        VIRTUAL auto delete_indices() -> void;

        VIRTUAL auto delete_post(const UUID::uuid& post_id) -> void;

        VIRTUAL auto delete_tag(const UUID::uuid& tag_id) -> void;

        VIRTUAL auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> void;

        VIRTUAL auto find_posts(
            const post_query& query
        ) -> search_result<UUID::uuid>;

        VIRTUAL auto find_tags(
            const tag_query& query
        ) -> search_result<UUID::uuid>;

        VIRTUAL auto remove_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> void;

        VIRTUAL auto update_post_date_modified(
            const UUID::uuid& post_id,
            decltype(post::date_modified) date_modified
        ) -> void;

        VIRTUAL auto update_post_description(const post_update& post) -> void;

        VIRTUAL auto update_post_title(const post_update& post) -> void;

        VIRTUAL auto update_tag_name(
            const UUID::uuid& tag_id,
            std::string_view old_name,
            std::string_view new_name
        ) -> void;
    };
}
