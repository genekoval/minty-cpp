#pragma once

#include "source.hpp"
#include "time_point.hpp"

#include <uuid++/uuid++>
#include <vector>
#include <zipline/zipline>

namespace minty {
    struct tag {
        UUID::uuid id;
        std::string name;
        std::vector<std::string> aliases;
        std::optional<std::string> description;
        std::optional<UUID::uuid> avatar;
        std::optional<UUID::uuid> banner;
        std::vector<source> sources;
        std::uint32_t post_count;
        time_point date_created;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::tag,
        &minty::tag::id,
        &minty::tag::name,
        &minty::tag::aliases,
        &minty::tag::description,
        &minty::tag::avatar,
        &minty::tag::banner,
        &minty::tag::sources,
        &minty::tag::post_count,
        &minty::tag::date_created
    );
}
