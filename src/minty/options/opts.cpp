#include "opts.h"

using namespace commline;

namespace minty::cli::opts {
    auto aliases() -> list<std::string_view> {
        return {
            {"a", "alias"},
            "Add a secondary name",
            "name"
        };
    }

    auto description() -> option<std::optional<std::string_view>> {
        return {
            {"d", "description"},
            "Set the description",
            "text"
        };
    }

    auto from() -> option<int> {
        return {
            {"f", "from"},
            "Result offset",
            "number",
            0
        };
    }

    auto links() -> list<std::string_view> {
        return {
            {"l", "link"},
            "Add a URL",
            "url"
        };
    }

    auto path() -> option<std::optional<std::string_view>> {
        return {
            {"S", "select"},
            "Select YAML output",
            "path"
        };
    }

    auto size() -> option<int> {
        return {
            {"n", "size"},
            "Result size",
            "number",
            10
        };
    }

    auto tags() -> list<std::string> {
        return {
            {"t", "tag"},
            "Post tags",
            "id"
        };
    }
}
