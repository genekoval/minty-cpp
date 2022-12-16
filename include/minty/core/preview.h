#pragma once

#include <minty/core/object_store.h>

namespace minty::core {
    auto generate_preview(
        bucket& bucket,
        const fstore::object& object
    ) -> ext::task<std::optional<UUID::uuid>>;

    auto initialize_image_previews() -> void;
}
