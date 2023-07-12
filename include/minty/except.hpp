#pragma once

#include <minty/model/object_preview.hpp>

#include <ext/string.h>
#include <fmt/format.h>
#include <stdexcept>
#include <zipline/zipline>

namespace minty {
    struct minty_error : zipline::zipline_error {
        using zipline_error::zipline_error;
    };

    struct invalid_data : minty_error {
        using minty_error::minty_error;
    };

    struct not_found : minty_error {
        using minty_error::minty_error;
    };

    class download_error : public minty_error {
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

        auto status() const noexcept -> std::int64_t;

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
