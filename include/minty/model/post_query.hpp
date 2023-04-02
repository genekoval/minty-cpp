#pragma once

#include "sort_order.hpp"
#include "visibility.hpp"

#include <uuid++/uuid++>
#include <zipline/zipline>

namespace minty {
    enum class post_sort_value : std::uint8_t {
        date_created,
        date_modified,
        relevance,
        title
    };

    struct post_sort {
        post_sort_value value;
        sort_order order;
    };

    struct post_query {
        std::uint32_t from = 0;
        std::uint32_t size = 0;
        std::optional<std::string> text;
        std::vector<UUID::uuid> tags;
        visibility visibility = visibility::pub;
        post_sort sort;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::post_sort,
        &minty::post_sort::order,
        &minty::post_sort::value
    );

    ZIPLINE_OBJECT(
        minty::post_query,
        &minty::post_query::from,
        &minty::post_query::size,
        &minty::post_query::text,
        &minty::post_query::tags,
        &minty::post_query::visibility,
        &minty::post_query::sort
    );
}
