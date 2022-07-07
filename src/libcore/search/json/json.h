#pragma once

#include <minty/repo/db/model.h>
#include <minty/error.h>

#include <nlohmann/json.hpp>
#include <uuid++/uuid++>

using json = nlohmann::json;

namespace nlohmann {
    template <typename Clock, typename Duration>
    struct adl_serializer<std::chrono::time_point<Clock, Duration>> {
        using time_point = std::chrono::time_point<Clock, Duration>;
        static auto from_json(const json& j, time_point& time) -> void {
            const auto value = j.get<int64_t>();
            time = time_point(Duration(value));
        }

        static auto to_json(json& j, const time_point& time) -> void {
            j = time.time_since_epoch().count();
        }
    };

    template <typename T>
    struct adl_serializer<std::optional<T>> {
        static auto from_json(const json& j, std::optional<T>& opt) -> void {
            if (j.is_null()) opt.reset();
            else opt = j.get<T>();
        }

        static auto to_json(json& j, const std::optional<T>& opt) -> void {
            if (!opt) j = nullptr;
            else j = opt.value();
        }
    };
}

namespace UUID {
    auto from_json(const nlohmann::json& j, uuid& u) -> void;

    auto to_json(nlohmann::json& j, const uuid& u) -> void;
}

namespace minty::repo::db {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        post_search,
        title,
        description,
        created,
        modified,
        tags
    )

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        tag_text,
        names
    )
}
