#include <internal/core/db/model/visibility.hpp>

namespace pg {
    auto enum_type<minty::visibility>::from_string(
        std::string_view string
    ) -> minty::visibility {
        using enum minty::visibility;

        if (string == "public") return pub;
        if (string == "draft") return draft;

        throw bad_conversion(fmt::format(
            R"(received invalid value for enum 'visibility': "{}")",
            string
        ));
    }

    auto enum_type<minty::visibility>::to_string(
        minty::visibility visibility
    ) -> std::string_view {
        return minty::to_string(visibility);
    }
}

PG_ENUM_DEFINE(minty::visibility);
