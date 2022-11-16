#pragma once

#include <minty/core/object_store.h>

namespace minty::core {
    auto generate_audio_preview(
        bucket& bucket,
        const fstore::object_meta& object
    ) -> ext::task<std::optional<UUID::uuid>>;

    auto generate_image_preview(
        bucket& bucket,
        const fstore::object_meta& object
    ) -> ext::task<std::optional<UUID::uuid>>;

    auto generate_image_preview(
        bucket& bucket,
        unsigned int width,
        unsigned int height,
        const void* pixels
    ) -> ext::task<UUID::uuid>;

    auto generate_video_preview(
        bucket& bucket,
        const fstore::object_meta& object
    ) -> ext::task<std::optional<UUID::uuid>>;
}
