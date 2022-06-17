#pragma once

#include "human/model.h"
#include "json/model.h"

#include <ext/string.h>
#include <iostream>

namespace minty::cli::output {
    using namespace std::literals;

    enum class format {
        id,
        json
    };

    constexpr auto format_string = std::array {
        "id"sv,
        "json"sv
    };

    template <typename T>
    auto human(const T& t) -> void {
        human_readable<T>::print(stderr, t);
    }

    template <typename T>
    auto id(
        std::ostream& out,
        const minty::core::search_result<T>& result
    ) -> void {
        for (const auto& hit : result.hits) {
            out << hit.id << "\n";
        }
    }

    template <typename T>
    auto json(std::ostream& out, const T& t) -> void {
        const auto json = static_cast<nlohmann::json>(t);
        out << json.dump(2) << std::endl;
    }

    template <typename T>
    auto entity(
        const T& t,
        bool print_json,
        bool print_human
    ) -> void {
        if (print_json) json(std::cout, t);
        if (print_human) human(t);
    }

    template <typename T>
    auto result(
        const minty::core::search_result<T>& result,
        const std::optional<format>& f,
        bool suppress_human_output
    ) -> void {
        if (!suppress_human_output) human(result);
        if (!f) return;

        auto& out = std::cout;

        switch (*f) {
            case format::id:
                id(out, result);
                break;
            case format::json:
                json(out, result);
                break;
        }
    }
}
