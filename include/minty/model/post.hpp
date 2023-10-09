#pragma once

#include "object_preview.hpp"
#include "post_preview.hpp"
#include "tag_preview.hpp"
#include "time_point.hpp"
#include "visibility.hpp"

#include <uuid++/uuid++>
#include <vector>
#include <zipline/zipline>

namespace minty {
    struct post {
        UUID::uuid id;
        std::string title;
        std::string description;
        minty::visibility visibility;
        time_point date_created;
        time_point date_modified;
        std::vector<object_preview> objects;
        std::vector<post_preview> posts;
        std::vector<tag_preview> tags;
        std::uint32_t comment_count;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        post,
        id,
        title,
        description,
        visibility,
        date_created,
        date_modified,
        objects,
        posts,
        tags,
        comment_count
    );
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::post,
        &minty::post::id,
        &minty::post::title,
        &minty::post::description,
        &minty::post::visibility,
        &minty::post::date_created,
        &minty::post::date_modified,
        &minty::post::objects,
        &minty::post::posts,
        &minty::post::tags,
        &minty::post::comment_count
    );
}
