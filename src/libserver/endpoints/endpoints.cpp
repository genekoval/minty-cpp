#include "endpoints.h"

#include <minty/net/zipline/transfer.h>

namespace minty::server::endpoint {
    auto add_creator(protocol& proto) -> void {
        const auto name = proto.read<std::string>();
        proto.reply(proto.api->add_creator(name));
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
        proto.reply(proto.info->version);
    }
}
