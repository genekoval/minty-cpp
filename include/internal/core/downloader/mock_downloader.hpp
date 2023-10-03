#pragma once

#include "downloader.hpp"

#include <gmock/gmock.h>

namespace minty::test {
    struct downloader : core::downloader {
        MOCK_METHOD(
            (ext::task<std::pair<http::status, fstore::object>>),
            fetch,
            (std::string_view url, core::bucket& bucket),
            (override)
        );

        MOCK_METHOD(
            ext::task<std::optional<UUID::uuid>>,
            get_site_icon,
            (std::string_view scheme,
             std::string_view host,
             core::bucket& bucket),
            (override)
        );
    };
}
