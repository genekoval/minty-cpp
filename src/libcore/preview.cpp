#include "preview/preview.h"

#include <minty/core/preview.h>

#include <unordered_map>

namespace {
    using preview_generator = auto (*)(
        minty::core::object_store&,
        const fstore::object_meta&
    ) -> std::optional<UUID::uuid>;

    const auto generators =
        std::unordered_map<std::string_view, preview_generator> {
            {"audio", minty::core::generate_audio_preview},
            {"image", minty::core::generate_image_preview},
            {"video", minty::core::generate_video_preview}
        };

    auto get_preview_generator(
        const fstore::object_meta& object
    ) -> preview_generator {
        auto generator = generators.find(object.type);

        if (generator == generators.end()) return nullptr;
        return generator->second;
    }
}

namespace minty::core {
    preview_service::preview_service(
        object_store& objects
    ) : objects(&objects) {
        initialize_image_previews();
    }

    auto preview_service::generate_preview(
        const fstore::object_meta& object
    ) -> std::optional<UUID::uuid> {
        auto* generator = get_preview_generator(object);

        if (!generator) {
            TIMBER_DEBUG(
                "No preview generator for type: {}",
                object.mime_type()
            );
            return {};
        }

        TIMBER_DEBUG(
            "Generating preview for object of type: {}",
            object.mime_type()
        );

        return generator(*objects, object);
    }
}
