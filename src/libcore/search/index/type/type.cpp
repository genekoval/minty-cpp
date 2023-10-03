#include "type.hpp"

namespace {
    namespace internal {
        const auto date = json({{"type", "date"}});

        const auto keyword = json({{"type", "keyword"}});

        const auto text = json({{"type", "text"}});
    }
}

namespace minty::core::search::type {
    auto date() -> json { return internal::date; }

    auto keyword() -> json { return internal::keyword; }

    auto text() -> json { return internal::text; }
}
