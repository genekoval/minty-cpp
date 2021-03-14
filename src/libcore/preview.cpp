#include "preview/preview.h"

#include <minty/core/preview.h>

#include <unordered_map>

namespace minty::core {
    using preview_generator =
        std::string (*)(fstore::bucket&, const fstore::object_meta&);

    constexpr auto jpeg = "image/jpeg";
    constexpr auto png = "image/png";

    const auto generators = std::unordered_map<
        std::string_view,
        preview_generator
    > {
        {jpeg, generate_image_preview},
        {png, generate_image_preview}
    };

    auto get_preview_generator(
        std::string_view mime_type
    ) -> preview_generator {
        auto generator = generators.find(mime_type.data());
        if (generator == generators.end()) return nullptr;
        return generator->second;
    }

    preview_service::preview_service(fstore::bucket& bucket) :
        bucket(&bucket)
    {
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

        return generator(*bucket, object);
    }
}
