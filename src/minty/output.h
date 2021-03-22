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
        const minty::core::creator& creator
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::creator_preview& creator
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::object& object
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::post& post
    ) -> Emitter&;
}
