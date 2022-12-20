#pragma once

#include "data_size.hpp"
#include "post_preview.hpp"
#include "source.hpp"
#include "time_point.hpp"

#include <uuid++/uuid++>
#include <vector>
#include <zipline/zipline>

namespace minty {
    struct object {
        UUID::uuid id;
        std::string hash;
        data_size size;
        std::string type;
        std::string subtype;
        time_point date_added;
        std::optional<UUID::uuid> preview_id;
        std::optional<source> src;
        std::vector<post_preview> posts;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::object,
        &minty::object::id,
        &minty::object::hash,
        &minty::object::size,
        &minty::object::type,
        &minty::object::subtype,
        &minty::object::date_added,
        &minty::object::preview_id,
        &minty::object::src,
        &minty::object::posts
    );
}
