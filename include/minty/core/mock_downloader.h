#pragma once

#include <minty/core/downloader.h>

#include <gmock/gmock.h>

namespace minty::test {
    struct downloader : core::downloader {
        MOCK_METHOD(ext::task<bool>, fetch, (
            std::string_view url,
            std::function<ext::task<>(harvest::stream&)>&& callback
        ), (override));

        MOCK_METHOD(ext::task<>, get_site_icon, (
            std::string_view url,
            std::function<ext::task<>(harvest::stream&)>&& pipe
        ), (override));
    };
}
