#pragma once

#include <minty/core/downloader.h>

#include <gmock/gmock.h>

namespace minty::test {
    struct downloader : core::downloader {
        MOCK_METHOD(bool, fetch, (
            std::string_view url,
            std::function<void(harvest::data_stream&)> callback
        ), (override));

        MOCK_METHOD(void, get_site_icon, (
            std::string_view url,
            std::function<void(harvest::data_stream&)> pipe
        ), (override));
    };
}
