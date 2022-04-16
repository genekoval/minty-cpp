#pragma once

#include <fstore/client.h>
#include <minty/minty>
#include <yaml-cpp/yaml.h>

namespace minty::cli {
    auto add_object(
        minty::api& api,
        std::string_view object
    ) -> core::object_preview;

    auto bucket() -> fstore::bucket;

    auto client() -> api;

    auto print_yaml(
        const YAML::Emitter& emitter,
        std::optional<std::string_view> path
    ) -> void;

    template <typename T>
    auto print(
        T&& t,
        std::optional<std::string_view> path = {}
    ) -> void {
        auto out = YAML::Emitter();
        out << t;
        print_yaml(out, path);
    }

    auto print_post(
        minty::api& api,
        const UUID::uuid& id,
        std::optional<std::string_view> path
    ) -> void;

    auto print_tag(
        minty::api& api,
        std::string_view id,
        std::optional<std::string_view> path
    ) -> void;
}
