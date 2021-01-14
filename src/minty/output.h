#pragma once

#include <minty/core/model.h>

#include <yaml-cpp/yaml.h>

namespace YAML {
    auto operator<<(
        Emitter& out,
        const minty::core::creator& creator
    ) -> Emitter& {
        out << BeginMap;

        out
            << Key << "id"
            << Value << creator.id

            << Key << "name"
            << Value << creator.name;

        if (!creator.aliases.empty()) {
            out
                << Key << "aliases"
                << Value << creator.aliases;
        }

        if (creator.bio.has_value()) {
            out
                << Key << "bio"
                << Value << creator.bio.value();
        }

        if (!creator.sources.empty()) {
            out << Key << "links" << Value << BeginSeq;

            for (const auto& source : creator.sources) {
                out << source.url;
            }

            out << EndSeq;
        }

        out
            << Key << "date added"
            << Value << creator.date_added;

        out << EndMap;
        return out;
    }
}
