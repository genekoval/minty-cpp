#include "opts.h"

using namespace commline;

namespace minty::cli::opts {
    auto add_tags() -> commline::list<UUID::uuid> {
        return {
            {"t", "tag"},
            "Add a tag",
            "id"
        };
    }

    auto description() -> option<std::string> {
        return {
            {"d", "description"},
            "Set the description",
            "text"
        };
    }

    auto from() -> option<unsigned int> {
        return {
            {"f", "from"},
            "Result offset",
            "number",
            0
        };
    }

    auto json() -> commline::flag {
        return {
            {"j", "json"},
            "Print result as JSON to STDOUT"
        };
    }

    auto output() -> option<std::optional<minty::cli::output::format>> {
        return {
            {"o", "output"},
            fmt::format(
                "Format of data printed to STDOUT ({})",
                fmt::join(minty::cli::output::format_string, " | ")
            ),
            "format"
        };
    }

    auto path() -> option<std::optional<std::string_view>> {
        return {
            {"S", "select"},
            "Select YAML output",
            "path"
        };
    }

    auto quiet() -> flag {
        return {
            {"q", "quiet"},
            "Suppress human-readable output"
        };
    }

    auto size() -> option<unsigned int> {
        return {
            {"n", "size"},
            "Result size",
            "number",
            10
        };
    }

    auto tags() -> list<UUID::uuid> {
        return {
            {"t", "tag"},
            "Post tags",
            "id"
        };
    }

    auto title() -> commline::option<std::string> {
        return {
            {"T", "title"},
            "Set the title",
            "text"
        };
    }
}
