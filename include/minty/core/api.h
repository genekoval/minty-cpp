#pragma once

#include <minty/core/downloader.h>
#include <minty/core/object_store.h>
#include <minty/core/search/search.h>
#include <minty/model/modification.hpp>
#include <minty/model/object.hpp>
#include <minty/model/object_preview.hpp>
#include <minty/model/post.hpp>
#include <minty/model/post_parts.hpp>
#include <minty/model/post_preview.hpp>
#include <minty/model/tag.hpp>
#include <minty/repo/db/database.h>

#include <ext/coroutine>

namespace minty::core {
    struct progress {
        std::size_t completed = 0;
        std::size_t total = 0;
    };

    struct source_parts {
        std::string site_id;
        std::string resource;
    };

    class api {
        repo::db::database* db;
        object_store* objects;
        downloader* dl;
        search_engine* search;

        auto add_object(
            bucket& bucket,
            fstore::object&& object,
            const std::optional<std::string>& src
        ) -> ext::task<object_preview>;

        auto add_site(
            std::string_view scheme,
            std::string_view host
        ) -> ext::task<std::string>;

        auto add_source(std::string_view url) -> ext::task<source>;

        auto get_posts(
            bucket& bucket,
            std::vector<repo::db::post_preview>&& posts
        ) -> ext::task<std::vector<post_preview>>;

        auto regenerate_preview(
            bucket& bucket,
            const repo::db::object_preview& object
        ) -> ext::task<bool>;

        auto regenerate_preview_task(
            netcore::thread_pool& workers,
            repo::db::object_preview obj,
            std::size_t& errors,
            progress& progress,
            netcore::event<>& finished
        ) -> ext::detached_task;
    public:
        api(
            repo::db::database& db,
            object_store& objects,
            downloader& dl,
            search_engine& search
        );

        auto add_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> comment_data;

        auto add_object_data(
            std::size_t stream_size,
            const fstore::add_object_fn auto& pipe
        ) -> ext::task<object_preview> {
            TIMBER_FUNC();

            auto bucket = co_await objects->connect();
            auto metadata = co_await bucket.add({}, stream_size, pipe);
            co_return co_await add_object(bucket, std::move(metadata), {});
        }

        auto add_objects_url(
            std::string_view url
        ) -> ext::task<std::vector<object_preview>>;

        auto add_post(post_parts parts) -> ext::task<UUID::uuid>;

        auto add_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            std::int16_t position
        ) -> ext::task<time_point>;

        auto add_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto add_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> comment_data;

        auto add_tag(std::string_view name) -> ext::task<UUID::uuid>;

        auto add_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<tag_name>;

        auto add_tag_source(
            const UUID::uuid& tag_id,
            std::string_view url
        ) -> ext::task<source>;

        auto delete_post(const UUID::uuid& id) -> ext::task<>;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<time_point>;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            std::span<range> ranges
        ) -> ext::task<decltype(post::date_modified)>;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> void;

        auto delete_tag(const UUID::uuid& id) -> ext::task<>;

        auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<tag_name>;

        auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::string_view source_id
        ) -> void;

        auto get_bucket_id() const noexcept -> const UUID::uuid&;

        auto get_comment(const UUID::uuid& comment_id) -> comment;

        auto get_comments(const UUID::uuid& post_id) -> comment_tree;

        auto get_object(const UUID::uuid& object_id) -> ext::task<object>;

        auto get_object_preview_errors() -> std::vector<object_error>;

        auto get_post(const UUID::uuid& id) -> ext::task<post>;

        auto get_posts(
            const post_query& query
        ) -> ext::task<search_result<post_preview>>;

        auto get_tag(const UUID::uuid& id) -> tag;

        auto get_tags(
            const tag_query& query
        ) -> ext::task<search_result<tag_preview>>;

        auto move_post_object(
            const UUID::uuid& post_id,
            unsigned int old_index,
            unsigned int new_index
        ) -> ext::task<>;

        auto move_post_objects(
            const UUID::uuid& post_id,
            const std::vector<UUID::uuid>& objects,
            const std::optional<UUID::uuid>& destination
        ) -> ext::task<time_point>;

        auto prune() -> ext::task<>;

        auto regenerate_preview(
            const UUID::uuid& object_id
        ) -> ext::task<std::optional<UUID::uuid>>;

        auto regenerate_previews(
            int jobs,
            progress& progress
        ) -> ext::task<std::size_t>;

        auto reindex() -> ext::task<>;

        auto set_comment_content(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> std::string;

        auto set_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> ext::task<modification<std::optional<std::string>>>;

        auto set_post_title(
            const UUID::uuid& post_id,
            std::string_view title
        ) -> ext::task<modification<std::optional<std::string>>>;

        auto set_tag_description(
            const UUID::uuid& tag_id,
            std::string_view description
        ) -> std::optional<std::string>;

        auto set_tag_name(
            const UUID::uuid& tag_id,
            std::string_view new_name
        ) -> ext::task<tag_name>;
    };
}
