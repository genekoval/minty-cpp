#include "preview.h"

#include <minty/core/preview.h>

#include <cstring>
#include <GraphicsMagick/Magick++.h>

namespace {
    constexpr auto thumbnail_format = "JPEG";
    constexpr auto thumbnail_size = 250;

    const auto thumbnail_geometry = Magick::Geometry(
        thumbnail_size,
        thumbnail_size
    );

    auto generate(
        minty::core::bucket& bucket,
        Magick::Image&& image
    ) -> ext::task<UUID::uuid> {
        const auto width = image.size().width();
        const auto height = image.size().height();

        TIMBER_DEBUG("Image dimensions: {} x {}", width, height);

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

            TIMBER_DEBUG(
                "Cropping image to: "
                "(width: {}, height: {}, xOffset: {}, yOffset: {})",
                crop.width(),
                crop.height(),
                crop.xOff(),
                crop.yOff()
            );


            image.crop(crop);
        }

        image.thumbnail(thumbnail_geometry);
        image.magick(thumbnail_format);

        auto out = Magick::Blob();
        image.write(&out);

        const auto obj = co_await bucket.add(
            {},
            out.length(),
            [&out](auto&& part) -> ext::task<> {
                co_await part.write(std::span(
                    reinterpret_cast<const std::byte*>(out.data()),
                    out.length()
                ));
            }
        );

        co_return obj.id;
    }
}

namespace minty::core {
    auto generate_image_preview(
        bucket& bucket,
        unsigned int width,
        unsigned int height,
        const void* pixels
    ) -> ext::task<UUID::uuid> {
        co_return co_await generate(bucket, Magick::Image(
            width,
            height,
            "RGB",
            MagickLib::StorageType::CharPixel,
            pixels
        ));
    }

    auto generate_image_preview(
        bucket& bucket,
        const fstore::object_meta& object
    ) -> ext::task<std::optional<UUID::uuid>> {
        auto* source = new std::byte[object.size];
        co_await bucket.get(object.id, source);

        auto blob = Magick::Blob();
        blob.updateNoCopy(source, object.size);

        co_return co_await generate(bucket, Magick::Image(blob));
    }

    auto initialize_image_previews() -> void {
        Magick::InitializeMagick(nullptr);
    }
}
