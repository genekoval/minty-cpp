#pragma once

#include <minty/test.h>

#include <harvest/harvest>

namespace minty::core {
    class downloader {
        harvest::client client;
    public:
        downloader() = default;

        downloader(std::string_view host, std::string_view port);

        VIRTUAL_DESTRUCTOR(downloader)

        VIRTUAL auto fetch(
            std::string_view url,
            std::function<ext::task<>(harvest::stream&)>&& callback
        ) -> ext::task<bool>;

        VIRTUAL auto get_site_icon(
            std::string_view url,
            std::function<ext::task<>(harvest::stream&)>&& pipe
        ) -> ext::task<>;
    };
}
