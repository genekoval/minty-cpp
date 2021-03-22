#include "output.h"

namespace YAML {
    auto operator<<(
        Emitter& out,
        const minty::core::comment& comment
    ) -> Emitter& {
        out
            << BeginMap
            << Key << "id" << Value << comment.id
            << Key << "content" << Value << comment.content
            << Key << "date posted" << Value << comment.date_created
            << EndMap;

        return out;
    }

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

    auto operator<<(
        Emitter& out,
        const minty::core::creator_preview& creator
    ) -> Emitter& {
        out
            << BeginMap
            << Key << "id" << Value << creator.id
            << Key << "name" << Value << creator.name
            << EndMap;

        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::object& object
    ) -> Emitter& {
        out << BeginMap;

        out
            << Key << "id" << Value << object.id
            << Key << "hash" << Value << object.hash
            << Key << "size" << Value << object.size
            << Key << "mime type" << Value << object.mime_type
            << Key << "date added" << Value << object.date_added;

        out << EndMap;
        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::post& post
    ) -> Emitter& {
        out << BeginMap;

        out << Key << "id" << Value << post.id;

        if (post.description.has_value()) {
            out
                << Key << "description"
                << Value << post.description.value();
        }

        out << Key << "date created" << Value << post.date_created;
        if (post.date_modified != post.date_created) {
            out << Key << "date modified" << Value << post.date_modified;
        }

        out << Key << "creators" << Value << BeginSeq;
        for (const auto& creator : post.creators) {
            out << creator;
        }
        out << EndSeq;

        if (!post.objects.empty()) {
            out << "objects" << Value << BeginSeq;

            for (const auto& object : post.objects) {
                out << object;
            }

            out << EndSeq;
        }

        out << EndMap;
        return out;
    }
}
