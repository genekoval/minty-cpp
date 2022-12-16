#pragma once

#include <internal/conf/settings.hpp>

#include <dbtools/dbtools>

namespace minty::cli {
    constexpr auto dump_file = "minty.dump";

    auto database(std::string_view confpath) -> dbtools::postgresql;

    auto database(const conf::settings& settings) -> dbtools::postgresql;
}
