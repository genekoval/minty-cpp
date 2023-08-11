#pragma once

#include "object_preview.hpp"
#include "time_point.hpp"

#include <ext/json.hpp>
#include <zipline/zipline>

namespace minty {
    struct post_preview {
        UUID::uuid id;
        std::optional<std::string> title;
        std::optional<object_preview> preview;
        std::uint32_t comment_count;
        std::uint32_t object_count;
        time_point date_created;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        post_preview,
        id,
        title,
        preview,
        comment_count,
        object_count,
        date_created
    );
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::post_preview,
        &minty::post_preview::id,
        &minty::post_preview::title,
        &minty::post_preview::preview,
        &minty::post_preview::comment_count,
        &minty::post_preview::object_count,
        &minty::post_preview::date_created
    );
}
