#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace minty::core::search::type {
    auto date() -> json;

    auto keyword() -> json;

    auto text() -> json;
}
