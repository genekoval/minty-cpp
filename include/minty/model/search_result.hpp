#pragma once

#include <nlohmann/json.hpp>
#include <zipline/zipline>

namespace minty {
    template <typename T>
    struct search_result {
        std::uint32_t total;
        std::vector<T> hits;
    };

    template <std::convertible_to<nlohmann::json> T>
    auto from_json(const nlohmann::json& j, search_result<T>& r) -> void {
        j["total"].get_to(r.total);
        j["hits"].get_to(r.hits);
    }

    template <std::convertible_to<nlohmann::json> T>
    auto to_json(nlohmann::json& j, const search_result<T>& r) -> void {
        j["total"] = r.total;
        j["hits"] = r.hits;
    }
}

namespace zipline {
    template <typename T, io::reader Reader>
    ZIPLINE_OBJECT_DECODER_TEMPLATE(
        minty::search_result<T>,
        &minty::search_result<T>::total,
        &minty::search_result<T>::hits
    );

    template <typename T, io::writer Writer>
    ZIPLINE_OBJECT_ENCODER_TEMPLATE(
        minty::search_result<T>,
        &minty::search_result<T>::total,
        &minty::search_result<T>::hits
    );
}
