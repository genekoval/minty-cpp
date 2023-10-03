#pragma once

#include <ext/json.hpp>
#include <uuid++/json.hpp>
#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct source {
        std::int64_t id;
        std::string url;
        std::optional<UUID::uuid> icon;

        auto operator==(const source&) const noexcept -> bool = default;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(source, id, url, icon);
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::source,
        &minty::source::id,
        &minty::source::url,
        &minty::source::icon
    );
}
