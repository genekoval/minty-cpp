#pragma once

#include <internal/core/object_store/object_store.hpp>

namespace minty::core {
    auto generate_preview(bucket& bucket, const fstore::object& object)
        -> ext::task<std::optional<UUID::uuid>>;

    auto initialize_image_previews() -> void;
}
