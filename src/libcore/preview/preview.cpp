#include "preview.hpp"

#include <unordered_map>

namespace {
    using preview_generator = auto (*)(
        minty::core::bucket&,
        const fstore::object&
    ) -> ext::task<std::optional<UUID::uuid>>;

    const auto generators =
        std::unordered_map<std::string_view, preview_generator> {
            {"audio", minty::core::generate_audio_preview},
            {"image", minty::core::generate_image_preview},
            {"video", minty::core::generate_video_preview}
        };

    auto get_preview_generator(
        const fstore::object& object
    ) -> preview_generator {
        auto generator = generators.find(object.type);

        if (generator == generators.end()) return nullptr;
        return generator->second;
    }
}

namespace minty::core {
    auto generate_preview(
        bucket& bucket,
        const fstore::object& object
    ) -> ext::task<std::optional<UUID::uuid>> {
        auto* generator = get_preview_generator(object);

        if (!generator) {
            TIMBER_DEBUG(
                "No preview generator for type: {}",
                object.mime_type()
            );
            co_return std::optional<UUID::uuid>();
        }

        TIMBER_DEBUG(
            "Generating preview for object of type: {}",
            object.mime_type()
        );

        co_return co_await generator(bucket, object);
    }
}
