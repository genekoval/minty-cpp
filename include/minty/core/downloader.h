#pragma once

#include <minty/test.h>

#include <harvest/harvest>

namespace minty::core {
    class downloader {
        std::optional<harvest::api> service;
    public:
        downloader() = default;

        downloader(std::string_view host, std::string_view port);

        VIRTUAL_DESTRUCTOR(downloader)

        VIRTUAL auto fetch(
            std::string_view url,
            std::function<void(harvest::stream_type&)> callback
        ) -> bool;

        VIRTUAL auto get_site_icon(
            std::string_view url,
            std::function<void(harvest::stream_type&)> pipe
        ) -> void;
    };
}
