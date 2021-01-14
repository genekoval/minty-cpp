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
}
