#pragma once

#include "type.h"

#include <minty/minty>
#include <nlohmann/json.hpp>

namespace minty::repo::db {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        tag_preview,
        id,
        name,
        avatar
    );
}

namespace minty::core {
    template <typename T>
    auto from_json(const nlohmann::json& j, search_result<T>& r) -> void {
        j.get_to(r.hits);
        j.get_to(r.total);
    }

    template <typename T>
    auto to_json(nlohmann::json& j, const search_result<T>& r) -> void {
        j["hits"] = r.hits;
        j["total"] = r.total;
    }

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        source,
        id,
        url,
        icon
    );

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        tag,
        id,
        name,
        aliases,
        description,
        avatar,
        banner,
        sources,
        post_count,
        date_created
    );
}
