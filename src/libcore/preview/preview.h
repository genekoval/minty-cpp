#pragma once

#include <fstore/client.h>

namespace minty::core {
    auto generate_image_preview(
        fstore::bucket& bucket,
        const fstore::object_meta& object
    ) -> std::string;

    auto generate_image_preview(
        fstore::bucket& bucket,
        std::span<std::byte> source
    ) -> std::string;

    auto generate_image_preview(
        fstore::bucket& bucket,
        unsigned int width,
        unsigned int height,
        const void* pixels
    ) -> std::string;

    auto generate_video_preview(
        fstore::bucket& bucket,
        const fstore::object_meta& object
    ) -> std::string;

    auto initialize_image_previews() -> void;
}
