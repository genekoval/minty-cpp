#include "preview.h"

#include <cstring>
#include <GraphicsMagick/Magick++.h>

namespace minty::core {
    constexpr auto thumbnail_format = "PNG";
    constexpr auto thumbnail_size = 250;

    const auto thumbnail_geometry = Magick::Geometry(
        thumbnail_size,
        thumbnail_size
    );

    auto generate_image_preview(
        fstore::bucket& bucket,
        const fstore::object_meta& object
    ) -> std::string {
        auto* source = new std::byte[object.size];
        bucket.get(object.id, source);

        auto blob = Magick::Blob();
        blob.updateNoCopy(source, object.size);

        auto image = Magick::Image(blob);
        auto size = image.size();

        DEBUG()
            << "image dimensions: "
            << size.width() << " x " << size.height();

        if (size.width() != size.height()) {
            const auto smaller = std::min(size.width(), size.height());
            const auto larger = std::max(size.width(), size.height());
            const auto offset = (larger - smaller) / 2;

            const auto crop = Magick::Geometry(
                smaller,
                smaller,
                smaller == size.height() ? offset : 0,
                smaller == size.width() ? offset : 0
            );

            DEBUG()
                << "cropping image to: (width: " << crop.width()
                << ", height: " << crop.height()
                << ", xOffset: " << crop.xOff()
                << ", yOffset: " << crop.yOff()
                << ")";

            image.crop(crop);
            size = image.size();
        }

        image.thumbnail(thumbnail_geometry);
        image.magick(thumbnail_format);

        auto out = Magick::Blob();
        image.write(&out);

        return bucket.add({}, out.length(), [&out](auto&& part) {
            part.write(std::span(
                reinterpret_cast<const std::byte*>(out.data()),
                out.length()
            ));
        }).id;
    }

    auto initialize_image_previews() -> void {
        Magick::InitializeMagick(nullptr);
    }
}
