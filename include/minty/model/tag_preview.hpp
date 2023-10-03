#pragma once

#include <ext/json.hpp>
#include <uuid++/json.hpp>
#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct tag_preview {
        UUID::uuid id;
        std::string name;
        std::optional<UUID::uuid> avatar;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(tag_preview, id, name, avatar);
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::tag_preview,
        &minty::tag_preview::id,
        &minty::tag_preview::name,
        &minty::tag_preview::avatar
    );
}
