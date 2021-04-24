#pragma once

#include <minty/minty>
#include <yaml-cpp/yaml.h>

namespace minty::cli {
    auto client() -> api;

    auto print_yaml(
        const YAML::Emitter& emitter,
        std::optional<std::string_view> path
    ) -> void;

    template <typename T>
    auto print(
        T&& t,
        std::optional<std::string_view> path
    ) -> void {
        auto out = YAML::Emitter();
        out << t;
        print_yaml(out, path);
    }
}
