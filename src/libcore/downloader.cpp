#include <internal/core/downloader/downloader.hpp>

#include <minty/except.hpp>

namespace minty::core {
    downloader::downloader(http::session& session) : session(&session) {}

    auto downloader::fetch(
        http::request& request,
        bucket& bucket
    ) -> ext::task<std::pair<http::status, fstore::object>> {
        auto task = read(request, bucket);

        const auto response = co_await request.perform(*session);
        auto object = co_await task;

        co_return std::make_pair(response.status(), std::move(object));
    }

    auto downloader::fetch(
        std::string_view url,
        bucket& bucket
    ) -> ext::task<std::pair<http::status, fstore::object>> {
        auto request = http::request();
        request.follow_redirects(true);
        request.url = url;
        co_return co_await fetch(request, bucket);
    }

    auto downloader::get_site_icon(
        std::string_view scheme,
        std::string_view host,
        bucket& bucket
    ) -> ext::task<std::optional<UUID::uuid>> {
        auto request = http::request();
        request.follow_redirects(true);

        auto& url = request.url;

        url.scheme(std::string(scheme));
        url.host(std::string(host));
        url.path("/favicon.ico");

        const auto [status, icon] = co_await fetch(request, bucket);

        if (status.ok() && icon.type == "image") co_return icon.id;

        co_await bucket.remove(icon.id);

        if (status != 404) {
            TIMBER_ERROR(
                "Failed to download site icon for {}://{}: "
                "{} {}: {}",
                scheme,
                host,
                request.method,
                request.url,
                status.ok() ?
                    fmt::format(
                        "Expected image; received {}",
                        icon.mime_type()
                    ) :
                    fmt::format("Response status {}", status.code)
            );
        }

        co_return std::nullopt;
    }
    auto downloader::read(
        http::request& request,
        bucket& bucket
    ) -> ext::jtask<fstore::object> {
        auto stream = request.pipe();
        auto chunk = co_await stream.read();

        const auto size = stream.expected_size();
        if (size < 0) throw minty_error("Content length is unknown");

        co_return co_await bucket.add(
            std::nullopt,
            size,
            [&](fstore::part& part) -> ext::task<> {
                while (!chunk.empty()) {
                    co_await part.write(chunk);
                    chunk = co_await stream.read();
                }
            }
        );
    }
}
