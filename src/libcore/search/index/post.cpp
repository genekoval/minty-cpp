#include "index.hpp"
#include "type/type.hpp"

namespace minty::core::search::config {
    auto post() -> std::string {
        return json({{"mappings",
                      {{"properties",
                        {{"title",
                          {{"type", "text"},
                           {"fields", {{"keyword", type::keyword()}}}}},
                         {"description", type::text()},
                         {"visibility", type::keyword()},
                         {"created", type::date()},
                         {"modified", type::date()},
                         {"tags", type::keyword()}}}}}}
        ).dump();
    }
}
