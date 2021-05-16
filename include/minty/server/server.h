#pragma once

#include <minty/core/api.h>
#include <minty/server/server_info.h>

namespace minty::server {
    auto listen(
        core::api& api,
        const server_info& info,
        std::string_view endpoint,
        std::function<void()>&& callback
    ) -> void;
}
