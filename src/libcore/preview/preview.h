#pragma once

#include <ext/dynarray.h>
#include <fstore/client.h>

namespace minty::core {
    auto generate_image_preview(
        fstore::bucket& bucket,
        const fstore::object_meta& object
    ) -> std::string;

    auto initialize_image_previews() -> void;
}
