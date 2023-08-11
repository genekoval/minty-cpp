#pragma once

#include "time_point.hpp"

#include <ext/json.hpp>
#include <zipline/zipline>

namespace minty {
    template <typename T>
    struct modification {
        time_point date_modified;
        T new_value;
    };

    template <std::convertible_to<nlohmann::json> T>
    auto from_json(const nlohmann::json& j, modification<T>& m) -> void {
        j["modified"].get_to(m.date_modified);
        j["value"].get_to(m.new_value);
    }

    template <std::convertible_to<nlohmann::json> T>
    auto to_json(nlohmann::json& j, const modification<T>& m) -> void {
        j["modified"] = m.date_modified;
        j["value"] = m.new_value;
    }
}

namespace zipline {
    template <typename T, io::reader Reader>
    ZIPLINE_OBJECT_DECODER_TEMPLATE(
        minty::modification<T>,
        &minty::modification<T>::date_modified,
        &minty::modification<T>::new_value
    );

    template <typename T, io::writer Writer>
    ZIPLINE_OBJECT_ENCODER_TEMPLATE(
        minty::modification<T>,
        &minty::modification<T>::date_modified,
        &minty::modification<T>::new_value
    );
}
