#pragma once

#include <minty/core/object_store.h>

namespace minty::core {
    class preview_service {
        object_store* objects;
    public:
        preview_service(object_store& objects);

        auto generate_preview(
            const fstore::object_meta& object
        ) -> std::optional<std::string>;
    };
}
