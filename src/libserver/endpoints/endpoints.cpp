#include "endpoints.h"

#include <minty/net/zipline/transfer.h>

namespace minty::server::endpoint {
    auto add_comment(protocol& proto) -> void {
        const auto post_id = proto.read<std::string>();
        const auto parent_id = proto.read<std::optional<std::string>>();
        const auto content = proto.read<std::string>();

        proto.reply(proto.api->add_comment(post_id, parent_id, content));
    }

    auto add_post(protocol& proto) -> void {
        auto req = proto.read<net::post_parts>();
        auto objects = std::vector<std::pair<double, std::string>>();

        for (auto& [index, blob] : req.blobs) {
            const auto id = proto.api->add_object_data(
                blob.size(),
                [&blob](auto&& part) {
                    blob.read([&part](auto&& chunk) { part.write(chunk); });
                });

            objects.push_back({index, id});
        }

        for (const auto& [index, file] : req.files) {
            objects.push_back({index, proto.api->add_object_local(file)});
        }

        for (const auto& [index, url] : req.urls) {
            const auto downloaded = proto.api->add_object_url(url);

            auto i = 0.0;

            for (const auto& obj : downloaded) {
                objects.push_back({index + i, obj});
                i += 0.1;
            }
        }

        std::sort(objects.begin(), objects.end());

        auto parts = core::post_parts {
            .title = req.title,
            .description = req.description,
            .tags = req.tags
        };

        for (const auto& obj : objects) {
            parts.objects.push_back(std::move(obj.second));
        }

        proto.reply(proto.api->add_post(parts));
    }

    auto add_tag(protocol& proto) -> void {
        const auto name = proto.read<std::string>();
        proto.reply(proto.api->add_tag(name));
    }

    auto get_comments(protocol& proto) -> void {
        const auto post_id = proto.read<std::string>();
        proto.reply(proto.api->get_comments(post_id));
    }

    auto get_post(protocol& proto) -> void {
        const auto id = proto.read<std::string>();
        proto.reply(proto.api->get_post(id));
    }

    auto get_server_info(protocol& proto) -> void {
        proto.reply(*(proto.info));
    }

    auto get_tag(protocol& proto) -> void {
        const auto id = proto.read<std::string>();
        proto.reply(proto.api->get_tag(id));
    }

    auto get_tag_posts(protocol& proto) -> void {
        const auto id = proto.read<std::string>();
        proto.reply(proto.api->get_tag_posts(id));
    }

    auto get_tag_previews(protocol& proto) -> void {
        proto.reply(proto.api->get_tag_previews());
    }
}
