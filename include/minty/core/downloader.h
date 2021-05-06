#pragma once

#include <harvest/harvest>

namespace minty::core {
    class downloader {
        harvest::api service;
    public:
        downloader(std::string_view host, std::string_view port);

        auto fetch(
            std::string_view url,
            std::function<void(harvest::stream_type&)> callback
        ) -> bool;

        auto get_site_icon(
            std::string_view url,
            std::function<void(harvest::stream_type&)> pipe
        ) -> void;
    };
}
