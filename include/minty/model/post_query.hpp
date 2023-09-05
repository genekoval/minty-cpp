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

    constexpr auto to_string(
        post_sort_value value
    ) noexcept -> std::string_view {
        using enum post_sort_value;

        switch (value) {
            case date_created: return "created";
            case date_modified: return "modified";
            case relevance: return "relevance";
            case title: return "title";
        }
    }

    constexpr auto default_sort_order(post_sort_value value) -> sort_order {
        if (value == post_sort_value::title) return sort_order::ascending;
        return sort_order::descending;
    }

    struct post_sort {
        static constexpr auto created() -> post_sort {
            return post_sort_value::date_created;
        }

        static constexpr auto modified() -> post_sort {
            return post_sort_value::date_modified;
        }

        static constexpr auto relevance() -> post_sort {
            return post_sort_value::relevance;
        }

        static constexpr auto title() -> post_sort {
            return post_sort_value::title;
        }

        post_sort_value value;
        sort_order order;

        constexpr post_sort() : post_sort(post_sort_value::date_created) {}

        constexpr post_sort(post_sort_value value) :
            post_sort(value, default_sort_order(value))
        {}

        constexpr post_sort(post_sort_value value, sort_order order) :
            value(value),
            order(order)
        {}

        auto operator==(
            const post_sort& other
        ) const noexcept -> bool = default;
    };

    struct post_query {
        std::uint32_t from = 0;
        std::uint32_t size = 0;
        std::optional<std::string> text;
        std::vector<UUID::uuid> tags;
        minty::visibility visibility = visibility::pub;
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

template <>
struct fmt::formatter<minty::post_sort_value> : formatter<std::string_view> {
    template <typename FormatContext>
    auto format(minty::post_sort_value value, FormatContext& ctx) const {
        return formatter<std::string_view>::format(
            minty::to_string(value),
            ctx
        );
    }
};
