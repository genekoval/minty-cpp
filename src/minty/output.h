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
        const minty::core::post& post
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::tag& tag
    ) -> Emitter&;

    auto operator<<(
        Emitter& out,
        const minty::core::tag_preview& tag
    ) -> Emitter&;
}
