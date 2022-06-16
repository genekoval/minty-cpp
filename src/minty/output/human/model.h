#pragma once

#include "style.h"

#include <minty/core/model.h>

#include <ext/math.h>
#include <fmt/color.h>

#define MINTY_HUMAN_READABLE(Type) \
    template <> \
    struct human_readable<Type> { \
        static auto print(std::FILE* f, const Type& type) -> void; \
    };

namespace minty::cli::output {
    template <typename T>
    struct human_readable {};

    MINTY_HUMAN_READABLE(repo::db::tag_preview)

    template <typename T>
    struct human_readable<core::search_result<T>> {
        static auto print(
            std::FILE* f,
            const core::search_result<T>& result
        ) -> void {
            if (result.total == 0) {
                fmt::print(f, "No matches found\n");
                return;
            }

            const auto size = result.hits.size();
            const auto digits = ext::digits(size);

            for (auto i = 1ul; i <= size; ++i) {
                const auto& hit = result.hits[i - 1];

                fmt::print(f, style::index, "{:>{}}", i, digits);
                fmt::print(f, " · ");
                human_readable<T>::print(f, hit);
                fmt::print(f, "\n");
            }

            fmt::print(f, style::result, "{}", size);
            fmt::print(f, " of ");
            fmt::print(f, style::result, "{}", result.total);
            fmt::print(f, " results\n");
        }
    };
}
