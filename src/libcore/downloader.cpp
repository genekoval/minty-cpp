#include <minty/core/downloader.h>

namespace minty::core {
    downloader::downloader(std::string_view host, std::string_view port) :
        service(harvest::api(host, port))
    {}

    auto downloader::fetch(
        std::string_view url,
        std::function<void(harvest::data_stream&)> callback
    ) -> bool {
        return service->scrape_url(url, [&callback](
            auto& stream,
            auto current,
            auto total
        ) {
            TIMBER_DEBUG("Downloading file {} of {}", current, total);
            callback(stream);
        });
    }

    auto downloader::get_site_icon(
        std::string_view url,
        std::function<void(harvest::data_stream&)> pipe
    ) -> void {
        service->get_site_icon(url, pipe);
    }
}
