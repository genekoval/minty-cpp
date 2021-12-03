#include <minty/core/downloader.h>

namespace minty::core {
    downloader::downloader(std::string_view host, std::string_view port) :
        service(harvest::api(host, port))
    {
        const auto info = service->get_server_info();
        DEBUG() << "Using harvest version " << info.server;
    }

    auto downloader::fetch(
        std::string_view url,
        std::function<void(harvest::stream_type&)> callback
    ) -> bool {
        return service->scrape_url(url, [&callback](
            auto& stream,
            auto current,
            auto total
        ) {
            DEBUG() << "Downloading file " << current << " of " << total;
            callback(stream);
        });
    }

    auto downloader::get_site_icon(
        std::string_view url,
        std::function<void(harvest::stream_type&)> pipe
    ) -> void {
        service->get_site_icon(url, pipe);
    }
}
