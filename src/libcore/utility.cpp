#include "utility.hpp"

#include <minty/except.hpp>

#include <ext/string.h>
#include <fmt/core.h>

using minty::invalid_data;

namespace {
    auto assert_text_no_newlines(std::string_view text, std::string_view label)
        -> void {
        if (text.find("\n") != std::string_view::npos) {
            throw invalid_data("{} must not contain newlines", label);
        }
    }

    auto assert_text_not_empty(std::string_view text, std::string_view label)
        -> void {
        if (text.empty()) { throw invalid_data("{} must not be empty", label); }
    }

    auto format_text(std::string_view text) -> std::string {
        return ext::replace(
            ext::trim(text),
            std::regex("\r"),
            [](const auto&) -> std::string { return "\n"; }
        );
    }
}

namespace minty::core {
    auto format_comment(std::string_view content) -> std::string {
        const auto formatted = format_text(content);

        assert_text_not_empty(formatted, "comment");

        return formatted;
    }

    auto format_description(std::string_view description) -> std::string {
        return format_text(description);
    }

    auto format_tag(std::string_view name) -> std::string {
        const auto formatted = format_text(name);

        assert_text_not_empty(formatted, "name");
        assert_text_no_newlines(formatted, "name");

        return formatted;
    }

    auto format_title(std::string_view title) -> std::string {
        const auto formatted = format_text(title);

        assert_text_no_newlines(formatted, "title");

        return formatted;
    }
}
