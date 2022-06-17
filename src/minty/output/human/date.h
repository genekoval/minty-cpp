#pragma once

#include <chrono>
#include <string>

namespace minty::cli::output {
    using clock = std::chrono::system_clock;

    auto format_date(const clock::time_point& date) -> std::string;

    auto format_duration(const clock::time_point& from) -> std::string;

    auto parse_date(const std::string& date) -> clock::time_point;
}
