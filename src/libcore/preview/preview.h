#pragma once

#include <minty/core/object_store.h>

namespace minty::core {
    auto generate_image_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::string;

    auto generate_image_preview(
        object_store& objects,
        std::span<std::byte> source
    ) -> std::string;

    auto generate_image_preview(
        object_store& objects,
        unsigned int width,
        unsigned int height,
        const void* pixels
    ) -> std::string;

    auto generate_video_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::string;

    auto initialize_image_previews() -> void;
}
