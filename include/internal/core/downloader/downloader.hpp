#pragma once

#include <internal/core/object_store/object_store.hpp>
#include <internal/test.hpp>

#include <http/http>

namespace minty::core {
    class downloader {
        http::client* client = nullptr;

        auto fetch(
            http::request& request,
            bucket& bucket
        ) -> ext::task<std::pair<http::status, fstore::object>>;

        auto read(
            http::request& request,
            bucket& bucket
        ) -> ext::jtask<fstore::object>;
    public:
        downloader() = default;

        explicit downloader(http::client& client);

        VIRTUAL_DESTRUCTOR(downloader)

        VIRTUAL auto fetch(
            std::string_view url,
            bucket& bucket
        ) -> ext::task<std::pair<http::status, fstore::object>>;

        VIRTUAL auto get_site_icon(
            std::string_view scheme,
            std::string_view host,
            bucket& bucket
        ) -> ext::task<std::optional<UUID::uuid>>;
    };
}
