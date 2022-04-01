#include <minty/client/api.h>
#include <minty/net/zipline/transfer.h>

namespace minty {
    api::api(std::string_view endpoint) : endpoint(endpoint) {}

    auto api::connect() -> client {
        return client(errors, net::socket(netcore::connect(endpoint)));
    }

    auto api::add_object_local(std::string_view path) -> std::string {
        return connect().send<std::string>(
            event::add_object_local,
            path
        );
    }

    auto api::add_post(const core::post_parts& parts) -> std::string {
        return connect().send<std::string>(event::add_post, parts);
    }

    auto api::add_post_objects(
        std::string_view post_id,
        std::span<std::string> objects,
        std::int16_t position
    ) -> std::string {
        return connect().send<std::string>(
            event::add_post_objects,
            post_id,
            objects,
            position
        );
    }

    auto api::add_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        connect().send<void>(
            event::add_post_tag,
            post_id,
            tag_id
        );
    }

    auto api::add_related_post(
        std::string_view post_id,
        std::string_view related
    ) -> void {
        return connect().send<void>(event::add_related_post, post_id, related);
    }

    auto api::add_tag(std::string_view name) -> std::string {
        return connect().send<std::string>(
            event::add_tag,
            name
        );
    }

    auto api::add_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> core::tag_name {
        return connect().send<core::tag_name>(
            event::add_tag_alias,
            tag_id,
            alias
        );
    }

    auto api::add_tag_source(
        std::string_view tag_id,
        std::string_view url
    ) -> core::source {
        return connect().send<core::source>(
            event::add_tag_source,
            tag_id,
            url
        );
    }

    auto api::delete_post(std::string_view id) -> void {
        connect().send<void>(event::delete_post, id);
    }

    auto api::delete_post_objects(
        std::string_view post_id,
        std::span<const std::string_view> objects
    ) -> std::string {
        return connect().send<std::string>(
            event::delete_post_objects,
            post_id,
            objects
        );
    }

    auto api::delete_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        connect().send<void>(event::delete_post_tag, post_id, tag_id);
    }

    auto api::delete_related_post(
        std::string_view post_id,
        std::string_view related
    ) -> void {
        connect().send<void>(event::delete_related_post, post_id, related);
    }

    auto api::delete_tag(std::string_view id) -> void {
        connect().send<void>(event::delete_tag, id);
    }

    auto api::delete_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> core::tag_name {
        return connect().send<core::tag_name>(
            event::delete_tag_alias,
            tag_id,
            alias
        );
    }

    auto api::delete_tag_source(
        std::string_view tag_id,
        std::string_view source_id
    ) -> void {
        connect().send<void>(
            event::delete_tag_source,
            tag_id,
            source_id
        );
    }

    auto api::get_comments(
        std::string_view post_id
    ) -> std::vector<core::comment> {
        return connect().send<std::vector<core::comment>>(
            event::get_comments,
            post_id
        );
    }

    auto api::get_object(std::string_view object_id) -> core::object {
        return connect().send<core::object>(event::get_object, object_id);
    }

    auto api::get_post(std::string_view id) -> core::post {
        return connect().send<core::post>(
            event::get_post,
            id
        );
    }

    auto api::get_posts(
        const core::post_query& query
    ) -> core::search_result<core::post_preview> {
        return connect().send<core::search_result<core::post_preview>>(
            event::get_posts,
            query
        );
    }

    auto api::get_server_info() -> server::server_info {
        return connect().send<server::server_info>(event::get_server_info);
    }

    auto api::get_tag(std::string_view id) -> core::tag {
        return connect().send<core::tag>(
            event::get_tag,
            id
        );
    }

    auto api::get_tags(
        const core::tag_query& query
    ) -> core::search_result<core::tag_preview> {
        return connect().send<core::search_result<core::tag_preview>>(
            event::get_tags,
            query
        );
    }

    auto api::move_post_objects(
        std::string_view post_id,
        std::span<const std::string_view> objects,
        std::optional<std::string_view> destination
    ) -> std::string {
        return connect().send<std::string>(
            event::move_post_objects,
            post_id,
            objects,
            destination
        );
    }

    auto api::set_post_description(
        std::string_view post_id,
        std::string_view description
    ) -> core::modification<std::optional<std::string>> {
        return connect().send<core::modification<std::optional<std::string>>>(
            event::set_post_description,
            post_id,
            description
        );
    }

    auto api::set_post_title(
        std::string_view post_id,
        std::string_view title
    ) -> core::modification<std::optional<std::string>> {
        return connect().send<core::modification<std::optional<std::string>>>(
            event::set_post_title,
            post_id,
            title
        );
    }

    auto api::set_tag_description(
        std::string_view tag_id,
        std::string_view description
    ) -> std::optional<std::string> {
        return connect().send<std::optional<std::string>>(
            event::set_tag_description,
            tag_id,
            description
        );
    }

    auto api::set_tag_name(
        std::string_view tag_id,
        std::string_view new_name
    ) -> core::tag_name {
        return connect().send<core::tag_name>(
            event::set_tag_name,
            tag_id,
            new_name
        );
    }
}
