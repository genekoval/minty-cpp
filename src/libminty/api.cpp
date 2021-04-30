#include <minty/client/api.h>
#include <minty/net/zipline/transfer.h>

namespace minty {
    api::api(std::string_view endpoint) : endpoint(endpoint) {}

    auto api::connect() -> client {
        return client(net::socket(netcore::connect(endpoint)));
    }

    auto api::add_post(
        std::optional<std::string_view> description,
        const std::vector<std::string>& files,
        std::optional<std::string_view> tag_id,
        const std::vector<std::string>& tags
    ) -> std::string {
        return connect().send<std::string>(
            event::add_post,
            description,
            files,
            tag_id,
            tags
        );
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
        connect().emit(event::delete_post, id);
    }

    auto api::delete_tag(std::string_view id) -> void {
        connect().emit(event::delete_tag, id);
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
        connect().emit(
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

    auto api::get_post(std::string_view id) -> core::post {
        return connect().send<core::post>(
            event::get_post,
            id
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

    auto api::get_tags_by_name(
        std::string_view search_term
    ) -> std::vector<core::tag_preview> {
        return connect().send<std::vector<core::tag_preview>>(
            event::get_tags_by_name,
            search_term
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
