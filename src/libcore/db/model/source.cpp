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
            .icon = website.icon
        };
    }
}
