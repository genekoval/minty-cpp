#include <internal/server/http/parser.hpp>

#include <minty/except.hpp>

namespace http {
    auto parser<minty::post_sort_value>::parse(
        std::string_view string
    ) -> minty::post_sort_value {
        using enum minty::post_sort_value;

        if (string == "created") return date_created;
        if (string == "modified") return date_modified;
        if (string == "title") return title;
        if (string == "relevance") return relevance;

        throw error_code(400, "Invalid sort value '{}'", string);
    }

    auto parser<minty::sort_order>::parse(
        std::string_view string
    ) -> minty::sort_order {
        if (string == "asc") return minty::sort_order::ascending;
        if (string == "desc") return minty::sort_order::descending;

        throw error_code(400, "Invalid sort order '{}'", string);
    }

    auto parser<minty::visibility>::parse(
        std::string_view string
    ) -> minty::visibility{
        if (string == "public") return minty::visibility::pub;
        if (string == "draft") return minty::visibility::draft;

        return minty::visibility::invalid;
    }
}
