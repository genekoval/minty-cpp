#pragma once

#include <ext/json.hpp>
#include <uuid++/json.hpp>
#include <uuid++/uuid++>
#include <uuid++/zipline>
#include <zipline/zipline>

namespace minty {
    struct object_preview {
        UUID::uuid id;
        std::optional<UUID::uuid> preview_id;
        std::string type;
        std::string subtype;

        auto operator==(const object_preview&) const noexcept -> bool = default;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        object_preview,
        id,
        preview_id,
        type,
        subtype
    );
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::object_preview,
        &minty::object_preview::id,
        &minty::object_preview::preview_id,
        &minty::object_preview::type,
        &minty::object_preview::subtype
    );
}
