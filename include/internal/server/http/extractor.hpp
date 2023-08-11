#pragma once

#include <ext/string.h>
#include <http/http>

namespace http::server::extractor {
    template <>
    struct data<std::vector<UUID::uuid>> {
        static auto read(
            request& request
        ) -> ext::task<std::vector<UUID::uuid>>;
    };
}
