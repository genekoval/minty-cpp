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

    auto context::add_post(net::post_parts parts) -> std::string {
        auto objects = std::vector<std::pair<double, std::string>>();

        for (auto& [index, blob] : parts.blobs) {
            const auto id = api->add_object_data(
                blob.size(),
                [&blob](auto&& part) {
                    blob.read([&part](auto&& chunk) {
                        part.write(chunk);
                    });
                }
            );

            objects.push_back({index, id});
        }

        for (const auto& [index, file] : parts.files) {
            objects.push_back({index, api->add_object_local(file)});
        }

        for (const auto& [index, url] : parts.urls) {
            const auto downloaded = api->add_object_url(url);

            auto i = 0.0;

            for (const auto& obj : downloaded) {
                objects.push_back({index + i, obj});
                i += 0.1;
            }
        }

        std::sort(objects.begin(), objects.end());

        auto post = core::post_parts {
            .title = parts.title,
            .description = parts.description,
            .tags = parts.tags
        };

        for (const auto& obj : objects) {
            post.objects.push_back(std::move(obj.second));
        }

        return api->add_post(post);
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

    auto context::get_server_info() -> server_info {
        return *info;
	}

    auto context::get_tag(std::string tag_id) -> core::tag {
        return api->get_tag(tag_id);
	}

    auto context::get_tag_posts(
        std::string tag_id
    ) -> std::vector<core::post_preview> {
        return api->get_tag_posts(tag_id);
    }

    auto context::get_tag_previews() -> std::vector<core::tag_preview> {
        return api->get_tag_previews();
    }

    auto context::get_tags_by_name(
        std::string search_term
    ) -> std::vector<core::tag_preview> {
        return api->get_tags_by_name(search_term);
    }

    auto context::set_post_description(
        std::string post_id,
        std::string description
    ) -> std::optional<std::string> {
        return api->set_post_description(post_id, description);
    }

    auto context::set_post_title(
        std::string post_id,
        std::string title
    ) -> std::optional<std::string> {
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
