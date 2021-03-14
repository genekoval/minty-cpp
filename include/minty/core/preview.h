#pragma once

#include <fstore/client.h>

namespace minty::core {
    class preview_service {
        fstore::bucket* bucket;
    public:
        preview_service(fstore::bucket& bucket);

        auto generate_preview(
            const fstore::object_meta& object
        ) -> std::optional<std::string>;
    };
}
