#pragma once

#include <fmt/color.h>

namespace minty::cli::output::style {
    constexpr auto indent = "     ";
    constexpr auto index = fmt::fg(fmt::color::orange_red);
    constexpr auto result = fmt::fg(fmt::color::yellow_green);
    constexpr auto secondary = fmt::fg(fmt::color::dim_gray);
}
