#pragma once

#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct post_parts {
        std::string title;
        std::string description;
        std::vector<UUID::uuid> objects;
        std::vector<UUID::uuid> tags;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::post_parts,
        &minty::post_parts::title,
        &minty::post_parts::description,
        &minty::post_parts::objects,
        &minty::post_parts::tags
    );
}
