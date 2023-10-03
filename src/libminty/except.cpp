#include <minty/except.hpp>

namespace minty {
    minty_error::minty_error() : runtime_error("minty error") {}

    minty_error::minty_error(const std::string& what) : runtime_error(what) {}

    invalid_data::invalid_data(const std::string& what) : runtime_error(what) {}

    not_found::not_found(const std::string& what) : runtime_error(what) {}

    auto not_found::http_code() const noexcept -> int { return 404; }

    download_error::download_error(
        std::string_view url,
        long status,
        const object_preview& object
    ) :
        runtime_error(fmt::format("Download failed ({}): {}", status, url)),
        source_url(url),
        code(status),
        object(object) {}

    auto download_error::data() const noexcept -> const object_preview& {
        return object;
    }

    auto download_error::encode(zipline::io::abstract_writer& writer) const
        -> ext::task<> {
        co_await zipline::encode(source_url, writer);
        co_await zipline::encode(code, writer);
        co_await zipline::encode(object, writer);
    }

    auto download_error::http_code() const noexcept -> int { return code; }

    auto download_error::url() const noexcept -> std::string_view {
        return source_url;
    }
}

namespace zipline {
    auto decoder<minty::download_error, io::abstract_reader>::decode(
        io::abstract_reader& reader
    ) -> ext::task<minty::download_error> {
        const auto url = co_await zipline::decode<std::string>(reader);
        const auto status = co_await zipline::decode<std::int64_t>(reader);
        auto object = co_await zipline::decode<minty::object_preview>(reader);

        co_return minty::download_error(url, status, std::move(object));
    }
}
