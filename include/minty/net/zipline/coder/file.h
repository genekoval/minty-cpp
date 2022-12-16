#pragma once

#include "../protocol.h"

namespace zipline {
    template <>
    struct encoder<fstore::file, minty::net::client_type> {
        static auto encode(
            const fstore::file& file,
            minty::net::client_type& client
        ) -> ext::task<> {
            return zipline::encode(file, client.inner);
        }
    };
}
