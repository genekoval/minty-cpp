#include "endpoints.h"

namespace minty::server::endpoint {
    auto add_creator(protocol& proto) -> void {
        const auto id = proto.read<std::string>();
        proto.reply(proto.api->add_creator(id));
    }
}
