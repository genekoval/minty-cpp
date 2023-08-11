#include <minty/model/post_query.hpp>

namespace minty {
    auto default_sort_order(post_sort_value value) -> sort_order {
        using enum post_sort_value;
        using enum sort_order;

        if (value == date_created || value == date_modified) return descending;
        return ascending;
    }
}
