#include "index.h"

#include "type/type.h"

namespace minty::core::search::config {
    auto tag() -> std::string {
        return json({
            {"mappings", {
                {"properties", {
                    {"names", {
                        {"type", "search_as_you_type"}
                    }}
                }}
            }}
        }).dump();
    }
}
