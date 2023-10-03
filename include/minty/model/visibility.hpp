#pragma once

#include <nlohmann/json.hpp>
#include <zipline/zipline>

namespace minty {
    enum class visibility : std::int32_t { invalid = -1, draft, pub };

    auto to_string(visibility visibility) -> std::string_view;

    static_assert(zipline::codable<visibility>);

    NLOHMANN_JSON_SERIALIZE_ENUM(
        visibility,
        {{visibility::invalid, nullptr},
         {visibility::draft, "draft"},
         {visibility::pub, "public"}}
    )
}

template <>
struct fmt::formatter<minty::visibility> : formatter<std::string_view> {
    template <typename FormatContext>
    auto format(minty::visibility visibility, FormatContext& ctx) const {
        const auto string = minty::to_string(visibility);
        return formatter<std::string_view>::format(string, ctx);
    }
};
