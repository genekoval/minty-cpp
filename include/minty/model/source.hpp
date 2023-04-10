#pragma once

#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct source {
        std::int64_t id;
        std::string url;
        std::optional<UUID::uuid> icon;

        auto operator==(const source&) const noexcept -> bool = default;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::source,
        &minty::source::id,
        &minty::source::url,
        &minty::source::icon
    );
}
