#include "preview/preview.h"

#include <minty/core/preview.h>

#include <ext/string.h>
#include <unordered_map>

namespace {
    using preview_generator =
        auto (*)(fstore::bucket&, const fstore::object_meta&) -> std::string;

    const auto generators =
        std::unordered_map<std::string_view, preview_generator> {
            {"image", minty::core::generate_image_preview},
            {"video", minty::core::generate_video_preview}
        };

    auto get_preview_generator(
        std::string_view mime_type
    ) -> preview_generator {
        const auto type = ext::split(mime_type, "/").front();

        auto generator = generators.find(type);

        if (generator == generators.end()) return nullptr;
        return generator->second;
    }
}

namespace minty::core {
    preview_service::preview_service(fstore::bucket& bucket) : bucket(&bucket) {
        initialize_image_previews();
    }

    auto preview_service::generate_preview(
        const fstore::object_meta& object
    ) -> std::optional<std::string> {
        auto* generator = get_preview_generator(object.mime_type);

        if (!generator) {
            DEBUG() << "No preview generator for type: " << object.mime_type;
            return {};
        }

        DEBUG()
            << "Generating preview for object of type: "
            << object.mime_type;

        try {
            return generator(*bucket, object);
        }
        catch (const std::runtime_error& ex) {
            ERROR()
                << "Failed to generate preview for object ("
                << object.id << "): " << ex.what();
        }

        return {};
    }
}
