#include "json.hpp"

namespace UUID {
    auto from_json(const json& j, uuid& u) -> void {
        u = uuid(j.get<std::string>());
    }

    auto to_json(json& j, const uuid& u) -> void {
        j = json(u.string());
    }
}
