#include "preview.h"

#include <cstring>
#include <GraphicsMagick/Magick++.h>

namespace {
    constexpr auto thumbnail_format = "PNG";
    constexpr auto thumbnail_size = 250;

    const auto thumbnail_geometry = Magick::Geometry(
        thumbnail_size,
        thumbnail_size
    );

    auto generate_preview(
        minty::core::object_store& objects,
        Magick::Image&& image
    ) -> std::string {
        const auto width = image.size().width();
        const auto height = image.size().height();

        DEBUG() << "image dimensions: " << width << " x " << height;

        // Crop the image to a square if it is not a square already
        if (width != height) {
            const auto smaller = std::min(width, height);
            const auto larger = std::max(width, height);
            const auto offset = (larger - smaller) / 2;

            const auto crop = Magick::Geometry(
                smaller,
                smaller,
                smaller == height ? offset : 0,
                smaller == width ? offset : 0
            );

            DEBUG()
                << "cropping image to: (width: " << crop.width()
                << ", height: " << crop.height()
                << ", xOffset: " << crop.xOff()
                << ", yOffset: " << crop.yOff()
                << ")";

            image.crop(crop);
        }

        image.thumbnail(thumbnail_geometry);
        image.magick(thumbnail_format);

        auto out = Magick::Blob();
        image.write(&out);

        return objects.add({}, out.length(), [&out](auto&& part) {
            part.write(std::span(
                reinterpret_cast<const std::byte*>(out.data()),
                out.length()
            ));
        }).id;
    }
}

namespace minty::core {
    auto generate_image_preview(
        object_store& objects,
        unsigned int width,
        unsigned int height,
        const void* pixels
    ) -> std::string {
        return generate_preview(objects, Magick::Image(
            width,
            height,
            "RGB",
            MagickLib::StorageType::CharPixel,
            pixels
        ));
    }

    auto generate_image_preview(
        object_store& objects,
        const fstore::object_meta& object
    ) -> std::optional<std::string> {
        auto* source = new std::byte[object.size];
        objects.get(object.id, source);

        auto blob = Magick::Blob();
        blob.updateNoCopy(source, object.size);

        return generate_preview(objects, Magick::Image(blob));
    }

    auto initialize_image_previews() -> void {
        Magick::InitializeMagick(nullptr);
    }
}
