#include "type.h"

using json = nlohmann::json;

namespace UUID {
    auto from_json(const nlohmann::json& j, uuid& u) -> void {
        u = uuid(j.get<std::string>());
    }

    auto to_json(json& j, const uuid& u) -> void {
        j = json(u.string());
    }
}
