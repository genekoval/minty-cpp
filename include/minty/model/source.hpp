#pragma once

#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct source {
        std::string id;
        std::string url;
        std::optional<UUID::uuid> icon;
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
