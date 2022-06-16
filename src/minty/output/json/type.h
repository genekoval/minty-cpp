#pragma once

#include <nlohmann/json.hpp>
#include <uuid++/uuid++>

namespace nlohmann {
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
