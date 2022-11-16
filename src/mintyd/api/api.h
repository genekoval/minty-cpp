#pragma once

#include <minty/core/api.h>
#include <minty/conf/settings.h>

namespace minty::cli {
    auto api(
        const conf::settings& settings,
        std::function<ext::task<>(core::api&)>&& action
    ) -> void;
}
