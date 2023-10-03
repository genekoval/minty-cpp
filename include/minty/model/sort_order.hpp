#pragma once

#include <fmt/format.h>

namespace minty {
    enum class sort_order : std::uint8_t { ascending, descending };

    constexpr auto to_string(sort_order order) noexcept -> std::string_view {
        using enum sort_order;

        switch (order) {
            case ascending: return "asc";
            case descending: return "desc";
        }
    }
}

template <>
struct fmt::formatter<minty::sort_order> : formatter<std::string_view> {
    template <typename FormatContext>
    auto format(minty::sort_order order, FormatContext& ctx) const {
        return formatter<std::string_view>::format(
            minty::to_string(order),
            ctx
        );
    }
};
