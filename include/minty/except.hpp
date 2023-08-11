#pragma once

#include <minty/model/object_preview.hpp>

#include <ext/string.h>
#include <fmt/format.h>
#include <http/http>
#include <stdexcept>
#include <zipline/zipline>

namespace minty {
    struct minty_error : zipline::zipline_error {
        minty_error();

        minty_error(const std::string& what);

        template <typename... Args>
        minty_error(
            fmt::format_string<Args...> format_string,
            Args&&... args
        ) :
            runtime_error(fmt::format(
                format_string,
                std::forward<Args>(args)...
            ))
        {}
    };

    struct invalid_data : minty_error {
        invalid_data(const std::string& what);


        template <typename... Args>
        invalid_data(
            fmt::format_string<Args...> format_string,
            Args&&... args
        ) :
            runtime_error(fmt::format(
                format_string,
                std::forward<Args>(args)...
            ))
        {}
    };

    struct not_found : minty_error, http::server::error {
        not_found(const std::string& what);

        template <typename... Args>
        not_found(fmt::format_string<Args...> format_string, Args&&... args) :
            runtime_error(fmt::format(
                format_string,
                std::forward<Args>(args)...
            ))
        {}

        auto http_code() const noexcept -> int override;
    };

    class download_error : public minty_error, public http::server::error {
        std::string source_url;
        std::int64_t code;
        object_preview object;
    public:
        download_error(
            std::string_view url,
            long status,
            const object_preview& object
        );

        auto data() const noexcept -> const object_preview&;

        auto encode(
            zipline::io::abstract_writer& writer
        ) const -> ext::task<> override;

        auto http_code() const noexcept -> int override;

        auto url() const noexcept -> std::string_view;
    };

    using error_list = zipline::error_list<
        invalid_data,
        not_found,
        download_error
    >;
}

namespace zipline {
    template <>
    struct decoder<minty::download_error, io::abstract_reader> {
        static auto decode(
            io::abstract_reader& reader
        ) -> ext::task<minty::download_error>;
    };
}
