#include "context.h"

namespace minty::server {
    context::context(core::api& api, const server_info& info) :
        api(&api),
        info(&info)
    {}

    auto context::add_comment(
        UUID::uuid post_id,
        std::string content
    ) -> core::comment {
        return api->add_comment(post_id, content);
	}

    auto context::add_object_data(
        net::data_stream stream
    ) -> core::object_preview {
        return api->add_object_data(stream.size(), [&stream](auto&& part) {
            stream.read([&part](auto&& chunk) {
                part.write(chunk);
            });
        });
    }

    auto context::add_object_local(std::string path) -> core::object_preview {
        return api->add_object_local(path);
    }

    auto context::add_objects_url(
        std::string url
    ) -> std::vector<core::object_preview> {
        return api->add_objects_url(url);
    }

    auto context::add_post(core::post_parts parts) -> UUID::uuid {
        return api->add_post(parts);
	}

    auto context::add_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects,
        std::int16_t position
    ) -> decltype(core::post::date_modified) {
        return api->add_post_objects(post_id, objects, position);
    }

    auto context::add_post_tag(
        UUID::uuid post_id,
        UUID::uuid tag_id
    ) -> void {
        api->add_post_tag(post_id, tag_id);
    }

    auto context::add_related_post(
        UUID::uuid post_id,
        UUID::uuid related
    ) -> void {
        api->add_related_post(post_id, related);
    }

    auto context::add_reply(
        UUID::uuid parent_id,
        std::string content
    ) -> core::comment {
        return api->add_reply(parent_id, content);
    }

    auto context::add_tag(std::string name) -> UUID::uuid {
        return api->add_tag(name);
	}

    auto context::add_tag_alias(
        UUID::uuid tag_id,
        std::string alias
    ) -> core::tag_name{
        return api->add_tag_alias(tag_id, alias);
    }

    auto context::add_tag_source(
        UUID::uuid tag_id,
        std::string url
    ) -> core::source {
        return api->add_tag_source(tag_id, url);
    }

    auto context::delete_post(UUID::uuid post_id) -> void {
        api->delete_post(post_id);
	}

    auto context::delete_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects
    ) -> decltype(core::post::date_modified) {
        return api->delete_post_objects(post_id, objects);
    }

    auto context::delete_post_objects_ranges(
        UUID::uuid post_id,
        std::vector<core::range> ranges
    ) -> decltype(core::post::date_modified) {
        return api->delete_post_objects(post_id, ranges);
    }

    auto context::delete_post_tag(
        UUID::uuid post_id,
        UUID::uuid tag_id
    ) -> void {
        api->delete_post_tag(post_id, tag_id);
    }

    auto context::delete_related_post(
        UUID::uuid post_id,
        UUID::uuid related
    ) -> void {
        api->delete_related_post(post_id, related);
    }

    auto context::delete_tag(UUID::uuid tag_id) -> void {
        api->delete_tag(tag_id);
    }

    auto context::delete_tag_alias(
        UUID::uuid tag_id,
        std::string alias
    ) -> core::tag_name {
        return api->delete_tag_alias(tag_id, alias);
    }

    auto context::delete_tag_source(
        UUID::uuid tag_id,
        std::string source_id
    ) -> void {
        api->delete_tag_source(tag_id, source_id);
    }

    auto context::get_comment(UUID::uuid comment_id) -> core::comment_detail {
        return api->get_comment(comment_id);
    }

    auto context::get_comments(UUID::uuid post_id) -> core::comment_tree {
        return api->get_comments(post_id);
	}

    auto context::get_object(UUID::uuid object_id) -> core::object {
        return api->get_object(object_id);
    }

    auto context::get_post(UUID::uuid post_id) -> core::post {
        return api->get_post(post_id);
	}

    auto context::get_posts(
        core::post_query query
    ) -> core::search_result<core::post_preview> {
        return api->get_posts(query);
    }

    auto context::get_server_info() -> server_info {
        return *info;
	}

    auto context::get_tag(UUID::uuid tag_id) -> core::tag {
        return api->get_tag(tag_id);
	}

    auto context::get_tags(
        core::tag_query query
    ) -> core::search_result<core::tag_preview> {
        return api->get_tags(query);
    }

    auto context::move_post_object(
        UUID::uuid post_id,
        std::uint32_t old_index,
        std::uint32_t new_index
    ) -> void {
        api->move_post_object(post_id, old_index, new_index);
    }

    auto context::move_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects,
        std::optional<UUID::uuid> destination
    ) -> decltype(core::post::date_modified) {
        return api->move_post_objects(post_id, objects, destination);
    }

    auto context::set_comment_content(
        UUID::uuid comment_id,
        std::string content
    ) -> std::string {
        return api->set_comment_content(comment_id, content);
    }

    auto context::set_post_description(
        UUID::uuid post_id,
        std::string description
    ) -> core::modification<std::optional<std::string>> {
        return api->set_post_description(post_id, description);
    }

    auto context::set_post_title(
        UUID::uuid post_id,
        std::string title
    ) -> core::modification<std::optional<std::string>> {
        return api->set_post_title(post_id, title);
    }

    auto context::set_tag_description(
        UUID::uuid tag_id,
        std::string description
    ) -> std::optional<std::string> {
        return api->set_tag_description(tag_id, description);
    }

    auto context::set_tag_name(
        UUID::uuid tag_id,
        std::string new_name
    ) -> core::tag_name {
        return api->set_tag_name(tag_id, new_name);
    }
}
