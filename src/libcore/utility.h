#pragma once

#include <string>

namespace minty::core {
    auto format_comment(std::string_view content) -> std::string;

    auto format_description(std::string_view description) -> std::string;

    auto format_tag(std::string_view name) -> std::string;

    auto format_title(std::string_view title) -> std::string;
}
