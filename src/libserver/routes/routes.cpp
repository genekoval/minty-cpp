#include "routes.h"

namespace minty::server::route {
    auto add_comment(
        context* ctx,
        std::string post_id,
        std::optional<std::string> parent_id,
        std::string content
    ) -> minty::core::comment {
        return ctx->api->add_comment(post_id, parent_id, content);
	}

    auto add_post(context* ctx, net::post_parts parts) -> std::string {
        auto objects = std::vector<std::pair<double, std::string>>();

        for (auto& [index, blob] : parts.blobs) {
            const auto id = ctx->api->add_object_data(
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
            objects.push_back({index, ctx->api->add_object_local(file)});
        }

        for (const auto& [index, url] : parts.urls) {
            const auto downloaded = ctx->api->add_object_url(url);

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

        return ctx->api->add_post(post);
	}

    auto add_tag(context* ctx, std::string name) -> std::string {
        return ctx->api->add_tag(name);
	}

    auto delete_post(context* ctx, std::string post_id) -> void {
        ctx->api->delete_post(post_id);
	}

    auto get_comments(context* ctx, std::string post_id) -> core::comment_tree {
        return ctx->api->get_comments(post_id);
	}

    auto get_post(context* ctx, std::string post_id) -> core::post {
        return ctx->api->get_post(post_id);
	}

    auto get_server_info(context* ctx) -> server_info {
        return *(ctx->info);
	}

    auto get_tag(context* ctx, std::string tag_id) -> core::tag {
        return ctx->api->get_tag(tag_id);
	}

    auto get_tags_by_name(
        context* ctx,
        std::string search_term
    ) -> std::vector<core::tag_preview> {
        return ctx->api->get_tags_by_name(search_term);
    }

    auto get_tag_posts(
        context* ctx,
        std::string tag_id
    ) -> std::vector<core::post_preview> {
        return ctx->api->get_tag_posts(tag_id);
    }

    auto get_tag_previews(context* ctx) -> std::vector<core::tag_preview> {
        return ctx->api->get_tag_previews();
    }
}
