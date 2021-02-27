#include "endpoints.h"

#include <minty/net/zipline/transfer.h>

namespace minty::server::endpoint {
    auto add_comment(protocol& proto) -> void {
        const auto post_id = proto.read<std::string>();
        const auto parent_id = proto.read<std::optional<std::string>>();
        const auto content = proto.read<std::string>();

        proto.reply(proto.api->add_comment(post_id, parent_id, content));
    }

    auto add_creator(protocol& proto) -> void {
        const auto name = proto.read<std::string>();
        proto.reply(proto.api->add_creator(name));
    }

    auto add_post(protocol& proto) -> void {
        auto description = proto.read<std::optional<std::string>>();
        auto files = proto.read<std::vector<std::string>>();
        auto creator = proto.read<std::optional<std::string>>();
        auto tags = proto.read<std::vector<std::string>>();

        proto.reply(proto.api->add_post(
            description,
            std::span<std::string>(files.begin(), files.end()),
            creator,
            tags
        ));
    }

    auto get_comments(protocol& proto) -> void {
        const auto post_id = proto.read<std::string>();
        proto.reply(proto.api->get_comments(post_id));
    }

    auto get_creator(protocol& proto) -> void {
        const auto id = proto.read<std::string>();
        proto.reply(proto.api->get_creator(id));
    }

    auto get_creator_posts(protocol& proto) -> void {
        const auto id = proto.read<std::string>();
        proto.reply(proto.api->get_creator_posts(id));
    }

    auto get_creator_previews(protocol& proto) -> void {
        proto.reply(proto.api->get_creator_previews());
    }

    auto get_post(protocol& proto) -> void {
        const auto id = proto.read<std::string>();
        proto.reply(proto.api->get_post(id));
    }

    auto get_server_info(protocol& proto) -> void {
        proto.reply(*(proto.info));
    }
}
