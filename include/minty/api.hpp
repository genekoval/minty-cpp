#pragma once

#include "event.hpp"

#include <minty/model/comment.hpp>
#include <minty/model/modification.hpp>
#include <minty/model/object.hpp>
#include <minty/model/object_preview.hpp>
#include <minty/model/post.hpp>
#include <minty/model/post_parts.hpp>
#include <minty/model/post_query.hpp>
#include <minty/model/search_result.hpp>
#include <minty/model/server_info.hpp>
#include <minty/model/source.hpp>
#include <minty/model/tag.hpp>
#include <minty/model/tag_name.hpp>
#include <minty/model/tag_preview.hpp>
#include <minty/model/tag_query.hpp>

#include <fstore/fstore>

namespace minty {
    using socket = fstore::net::socket;

    using client_type = zipline::client<socket, event>;

    class api {
        std::unique_ptr<client_type> client;
    public:
        api() = default;

        api(netcore::socket&& socket);

        auto add_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> ext::task<comment_data>;

        auto add_object_data(
            const std::filesystem::path& path
        ) -> ext::task<object_preview>;

        auto add_objects(
            std::span<const std::string_view> arguments
        ) -> ext::task<std::vector<object_preview>>;

        auto add_objects_url(
            std::string_view url
        ) -> ext::task<std::vector<object_preview>>;

        auto add_post(const post_parts& parts) -> ext::task<UUID::uuid>;

        auto add_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            std::int16_t position
        ) -> ext::task<time_point>;

        auto add_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<>;

        auto add_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> ext::task<comment_data>;

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
            std::span<const UUID::uuid> objects
        ) -> ext::task<time_point>;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<>;

        auto delete_tag(const UUID::uuid& id) -> ext::task<>;

        auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<tag_name>;

        auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::int64_t source_id
        ) -> ext::task<>;

        auto get_comment(const UUID::uuid& comment_id) -> ext::task<comment>;

        auto get_comments(
            const UUID::uuid& post_id
        ) -> ext::task<std::vector<comment_data>>;

        auto get_object(const UUID::uuid& object_id) -> ext::task<object>;

        auto get_post(const UUID::uuid& id) -> ext::task<post>;

        auto get_posts(
            const post_query& query
        ) -> ext::task<search_result<post_preview>>;

        auto get_server_info() -> ext::task<server_info>;

        auto get_tag(const UUID::uuid& id) -> ext::task<tag>;

        auto get_tags(
            const tag_query& query
        ) -> ext::task<search_result<tag_preview>>;

        auto move_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            const std::optional<UUID::uuid>& destination
        ) -> ext::task<time_point>;

        auto set_comment_content(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> ext::task<std::string>;

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
        ) -> ext::task<std::optional<std::string>>;

        auto set_tag_name(
            const UUID::uuid& tag_id,
            std::string_view new_name
        ) -> ext::task<tag_name>;
    };
}
