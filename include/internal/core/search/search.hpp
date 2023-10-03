#pragma once

#include "index.hpp"

#include <internal/core/db/model/post_search.hpp>
#include <internal/core/db/model/post_update.hpp>
#include <internal/core/db/model/tag_search.hpp>
#include <internal/test.hpp>
#include <minty/model/post_query.hpp>
#include <minty/model/search_result.hpp>
#include <minty/model/tag_query.hpp>

#include <elasticsearch/elasticsearch>

namespace minty::core {
    class search_engine {
        friend class index;

        elastic::elasticsearch client;

        template <typename T>
        auto bulk_index(std::string_view index, std::span<const T> documents)
            -> ext::task<std::vector<std::string>>;

        auto create_index(const index& index) -> ext::task<>;

        auto delete_indices(std::initializer_list<std::string_view> index)
            -> ext::task<>;

        auto search(std::string_view index, const elastic::json& query)
            -> ext::task<search_result<UUID::uuid>>;
    public:
        index post_index;
        index tag_index;

        search_engine() = default;

        search_engine(
            http::session& session,
            std::string_view ns,
            std::string_view host,
            std::string_view api_key
        );

        VIRTUAL_DESTRUCTOR(search_engine)

        VIRTUAL auto add_post(const db::post_search& post) -> ext::task<>;

        VIRTUAL auto add_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        VIRTUAL auto add_posts(std::span<const db::post_search> posts)
            -> ext::task<std::vector<std::string>>;

        VIRTUAL auto add_tags(std::span<const db::tag_search> tags)
            -> ext::task<std::vector<std::string>>;

        VIRTUAL auto add_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<>;

        VIRTUAL auto create_indices() -> ext::task<>;

        VIRTUAL auto delete_indices() -> ext::task<>;

        VIRTUAL auto delete_post(const UUID::uuid& post_id) -> ext::task<>;

        VIRTUAL auto delete_tag(const UUID::uuid& tag_id) -> ext::task<>;

        VIRTUAL auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<>;

        VIRTUAL auto find_posts(const post_query& query)
            -> ext::task<search_result<UUID::uuid>>;

        VIRTUAL auto find_tags(const tag_query& query)
            -> ext::task<search_result<UUID::uuid>>;

        VIRTUAL auto publish_post(
            const UUID::uuid& post_id,
            time_point timestamp
        ) -> ext::task<>;

        VIRTUAL auto remove_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        VIRTUAL auto update_post_date_modified(
            const UUID::uuid& post_id,
            time_point date_modified
        ) -> ext::task<>;

        VIRTUAL auto update_post_description(const db::post_update& post)
            -> ext::task<>;

        VIRTUAL auto update_post_title(const db::post_update& post)
            -> ext::task<>;

        VIRTUAL auto update_tag_name(
            const UUID::uuid& tag_id,
            std::string_view old_name,
            std::string_view new_name
        ) -> ext::task<>;
    };
}
