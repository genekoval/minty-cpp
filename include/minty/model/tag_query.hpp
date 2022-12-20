#pragma once

#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    struct tag_query {
        std::uint32_t from = 0;
        std::uint32_t size = 0;
        std::string name;
        std::vector<UUID::uuid> exclude;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::tag_query,
        &minty::tag_query::from,
        &minty::tag_query::size,
        &minty::tag_query::name,
        &minty::tag_query::exclude
    );
}
