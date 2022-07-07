#include "index.h"
#include "type/type.h"

namespace minty::core::search::config {
    auto post() -> std::string {
        return json({
            {"mappings", {
                {"properties", {
                    {"title", {
                        {"type", "text"},
                        {"fields", {
                            {"keyword", type::keyword()}
                        }}
                    }},
                    {"description", type::text()},
                    {"created", type::date()},
                    {"modified", type::date()},
                    {"tags", type::keyword()}
                }}
            }}
        }).dump();
    }
}
