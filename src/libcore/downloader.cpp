#include <internal/core/downloader/downloader.hpp>

namespace minty::core {
    downloader::downloader(std::string_view endpoint) : client(endpoint) {}

    auto downloader::fetch(
        std::string_view url,
        std::function<ext::task<>(harvest::stream&)>&& callback
    ) -> ext::task<bool> {
        auto api = co_await client.connect();

        co_return co_await api->scrape_url(url, [&callback](
            auto& stream,
            auto current,
            auto total
        ) -> ext::task<> {
            TIMBER_DEBUG("Downloading file {} of {}", current, total);
            co_await callback(stream);
        });
    }

    auto downloader::get_site_icon(
        std::string_view url,
        std::function<ext::task<>(harvest::stream&)>&& pipe
    ) -> ext::task<> {
        auto api = co_await client.connect();

        co_await api->get_site_icon(url, pipe);
    }
}
