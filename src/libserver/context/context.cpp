#include "context.h"

namespace minty::server {
    context::context(core::api& api, const server_info& info) :
        api(&api),
        info(&info)
    {}

    auto context::add_comment(
        std::string post_id,
        std::optional<std::string> parent_id,
        std::string content
    ) -> core::comment {
        return api->add_comment(post_id, parent_id, content);
	}

    auto context::add_object_data(net::data_stream stream) -> std::string {
        return api->add_object_data(
            stream.size(),
            [&stream](auto&& part) {
                stream.read([&part](auto&& chunk) {
                    part.write(chunk);
                });
            }
        );
    }

    auto context::add_object_local(std::string path) -> std::string {
        return api->add_object_local(path);
    }

    auto context::add_objects_url(std::string url) -> std::vector<std::string> {
        return api->add_objects_url(url);
    }

    auto context::add_post(core::post_parts parts) -> std::string {
        return api->add_post(parts);
	}

    auto context::add_post_objects(
        std::string post_id,
        std::vector<std::string> objects,
        unsigned int position
    ) -> std::vector<core::object_preview> {
        return api->add_post_objects(post_id, objects, position);
    }

    auto context::add_post_tag(
        std::string post_id,
        std::string tag_id
    ) -> void {
        api->add_post_tag(post_id, tag_id);
    }

    auto context::add_tag(std::string name) -> std::string {
        return api->add_tag(name);
	}

    auto context::add_tag_alias(
        std::string tag_id,
        std::string alias
    ) -> core::tag_name{
        return api->add_tag_alias(tag_id, alias);
    }

    auto context::add_tag_source(
        std::string tag_id,
        std::string url
    ) -> core::source {
        return api->add_tag_source(tag_id, url);
    }

    auto context::delete_post(std::string post_id) -> void {
        api->delete_post(post_id);
	}

    auto context::delete_post_objects(
        std::string post_id,
        std::vector<std::string> objects
    ) -> std::string {
        return api->delete_post_objects(post_id, objects);
    }

    auto context::delete_post_objects_ranges(
        std::string post_id,
        std::vector<core::range> ranges
    ) -> std::string {
        return api->delete_post_objects(post_id, ranges);
    }

    auto context::delete_post_tag(
        std::string post_id,
        std::string tag_id
    ) -> void {
        api->delete_post_tag(post_id, tag_id);
    }

    auto context::delete_tag(std::string tag_id) -> void {
        api->delete_tag(tag_id);
    }

    auto context::delete_tag_alias(
        std::string tag_id,
        std::string alias
    ) -> core::tag_name {
        return api->delete_tag_alias(tag_id, alias);
    }

    auto context::delete_tag_source(
        std::string tag_id,
        std::string source_id
    ) -> void {
        api->delete_tag_source(tag_id, source_id);
    }

    auto context::get_comments(std::string post_id) -> core::comment_tree {
        return api->get_comments(post_id);
	}

    auto context::get_object(std::string object_id) -> core::object {
        return api->get_object(object_id);
    }

    auto context::get_post(std::string post_id) -> core::post {
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

    auto context::get_tag(std::string tag_id) -> core::tag {
        return api->get_tag(tag_id);
	}

    auto context::get_tags(
        core::tag_query query
    ) -> core::search_result<core::tag_preview> {
        return api->get_tags(query);
    }

    auto context::move_post_object(
        std::string post_id,
        std::uint32_t old_index,
        std::uint32_t new_index
    ) -> void {
        api->move_post_object(post_id, old_index, new_index);
    }

    auto context::set_comment_content(
        std::string comment_id,
        std::string content
    ) -> std::string {
        return api->set_comment_content(comment_id, content);
    }

    auto context::set_post_description(
        std::string post_id,
        std::string description
    ) -> core::modification<std::optional<std::string>> {
        return api->set_post_description(post_id, description);
    }

    auto context::set_post_title(
        std::string post_id,
        std::string title
    ) -> core::modification<std::optional<std::string>> {
        return api->set_post_title(post_id, title);
    }

    auto context::set_tag_description(
        std::string tag_id,
        std::string description
    ) -> std::optional<std::string> {
        return api->set_tag_description(tag_id, description);
    }

    auto context::set_tag_name(
        std::string tag_id,
        std::string new_name
    ) -> core::tag_name {
        return api->set_tag_name(tag_id, new_name);
    }
}
