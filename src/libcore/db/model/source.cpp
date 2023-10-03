#include <internal/core/db/model/source.hpp>

namespace minty::core::db {
    source::operator minty::source() const {
        return {
            .id = id,
            .url = fmt::format(
                "{}://{}{}",
                website.scheme,
                website.host,
                resource
            ),
            .icon = website.icon};
    }
}

PGCPP_COMPOSITE_DEFINE(
    minty::core::db::source,
    &minty::core::db::source::id,
    &minty::core::db::source::resource,
    &minty::core::db::source::website
);
