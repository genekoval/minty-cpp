#pragma once

#include <minty/core/object_store.h>

namespace minty::core {
    auto generate_audio_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::optional<UUID::uuid>;

    auto generate_image_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::optional<UUID::uuid>;

    auto generate_image_preview(
        object_store& objects,
        unsigned int width,
        unsigned int height,
        const void* pixels
    ) -> UUID::uuid;

    auto generate_video_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::optional<UUID::uuid>;

    auto initialize_image_previews() -> void;
}
