#include <minty/client/api.h>
#include <minty/error.h>
#include <minty/net/zipline/transfer.h>

namespace {
    auto get_socket_generator(
        std::string_view endpoint
    ) -> std::function<netcore::socket()> {
        if (endpoint.starts_with("/")) {
            // The endpoint is a Unix domain socket path.
            return [path = std::string(endpoint)] {
                return netcore::connect(path);
            };
        }

        // The endpoint is a host and port separated by a colon.
        const auto delim = endpoint.find(":");

        if (delim == decltype(endpoint)::npos) {
            throw minty::minty_error(
                "port missing from endpoint: {}",
                endpoint
            );
        }

        return [
            host = std::string(endpoint.substr(0, delim)),
            port = std::string(endpoint.substr(delim + 1))
        ] {
            return netcore::connect(host, port);
        };
    }
}

namespace minty {
    api::api(std::string_view endpoint) :
        socket(get_socket_generator(endpoint))
    {}

    auto api::connect() -> client {
        return client(errors, net::socket(socket()));
    }

    auto api::add_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> core::comment {
        return connect().send<core::comment>(
            event::add_comment,
            post_id,
            content
        );
    }

    auto api::add_object_local(std::string_view path) -> core::object_preview {
        return connect().send<core::object_preview>(
            event::add_object_local,
            path
        );
    }

    auto api::add_post(const core::post_parts& parts) -> UUID::uuid {
        return connect().send<UUID::uuid>(event::add_post, parts);
    }

    auto api::add_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
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
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> void {
        connect().send<void>(
            event::add_post_tag,
            post_id,
            tag_id
        );
    }

    auto api::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        return connect().send<void>(event::add_related_post, post_id, related);
    }

    auto api::add_reply(
        const UUID::uuid& parent_id,
        std::string_view content
    ) -> core::comment {
        return connect().send<core::comment>(
            event::add_reply,
            parent_id,
            content
        );
    }

    auto api::add_tag(std::string_view name) -> UUID::uuid {
        return connect().send<UUID::uuid>(
            event::add_tag,
            name
        );
    }

    auto api::add_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> core::tag_name {
        return connect().send<core::tag_name>(
            event::add_tag_alias,
            tag_id,
            alias
        );
    }

    auto api::add_tag_source(
        const UUID::uuid& tag_id,
        std::string_view url
    ) -> core::source {
        return connect().send<core::source>(
            event::add_tag_source,
            tag_id,
            url
        );
    }

    auto api::delete_post(const UUID::uuid& id) -> void {
        connect().send<void>(event::delete_post, id);
    }

    auto api::delete_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> std::string {
        return connect().send<std::string>(
            event::delete_post_objects,
            post_id,
            objects
        );
    }

    auto api::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> void {
        connect().send<void>(event::delete_post_tag, post_id, tag_id);
    }

    auto api::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        connect().send<void>(event::delete_related_post, post_id, related);
    }

    auto api::delete_tag(std::string_view id) -> void {
        connect().send<void>(event::delete_tag, id);
    }

    auto api::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> core::tag_name {
        return connect().send<core::tag_name>(
            event::delete_tag_alias,
            tag_id,
            alias
        );
    }

    auto api::delete_tag_source(
        const UUID::uuid& tag_id,
        std::string_view source_id
    ) -> void {
        connect().send<void>(
            event::delete_tag_source,
            tag_id,
            source_id
        );
    }

    auto api::get_comments(
        const UUID::uuid& post_id
    ) -> std::vector<core::comment> {
        return connect().send<std::vector<core::comment>>(
            event::get_comments,
            post_id
        );
    }

    auto api::get_object(const UUID::uuid& object_id) -> core::object {
        return connect().send<core::object>(event::get_object, object_id);
    }

    auto api::get_post(const UUID::uuid& id) -> core::post {
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

    auto api::get_tag(const UUID::uuid& id) -> core::tag {
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
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
        const std::optional<UUID::uuid>& destination
    ) -> std::string {
        return connect().send<std::string>(
            event::move_post_objects,
            post_id,
            objects,
            destination
        );
    }

    auto api::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> core::modification<std::optional<std::string>> {
        return connect().send<core::modification<std::optional<std::string>>>(
            event::set_post_description,
            post_id,
            description
        );
    }

    auto api::set_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> core::modification<std::optional<std::string>> {
        return connect().send<core::modification<std::optional<std::string>>>(
            event::set_post_title,
            post_id,
            title
        );
    }

    auto api::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> std::optional<std::string> {
        return connect().send<std::optional<std::string>>(
            event::set_tag_description,
            tag_id,
            description
        );
    }

    auto api::set_tag_name(
        const UUID::uuid& tag_id,
        std::string_view new_name
    ) -> core::tag_name {
        return connect().send<core::tag_name>(
            event::set_tag_name,
            tag_id,
            new_name
        );
    }
}
