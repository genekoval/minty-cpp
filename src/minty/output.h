#pragma once

#include <minty/core/model.h>

#include <yaml-cpp/yaml.h>

namespace YAML {
    auto operator<<(
        Emitter& out,
        const minty::core::comment& comment
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::data_size& data_size
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::object& object
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::object_preview& object
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::post& post
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::post_preview& post
    ) -> Emitter&;

    template <typename T>
    auto operator<<(
        Emitter& out,
        const minty::core::search_result<T> result
    ) -> Emitter& {
        out << BeginMap;

        out << Key << "total" << Value << result.total;

        out << Key << "hits" << Value << BeginSeq;
        for (const auto& hit : result.hits) out << hit;
        out << EndSeq;

        out << EndMap;
        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::tag& tag
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::tag_preview& tag
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const UUID::uuid& uuid
    ) -> Emitter&;
}
