#pragma once

#include <minty/conf/settings.h>

#include <dbtools/dbtools>

namespace minty::cli {
    constexpr auto dump_file = "minty.dump";

    auto database(std::string_view confpath) -> dbtools::postgresql;

    auto database(const conf::settings& settings) -> dbtools::postgresql;
}
