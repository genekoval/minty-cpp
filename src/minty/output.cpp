#include "output.h"

#include <fmt/format.h>

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
        const minty::core::data_size& data_size
    ) -> Emitter& {
        out
            << BeginMap
            << Key << "formatted" << Value << data_size.formatted
            << Key << "bytes" << Value << data_size.bytes
            << EndMap;

        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::tag& tag
    ) -> Emitter& {
        out << BeginMap;

        out
            << Key << "id"
            << Value << tag.id

            << Key << "name"
            << Value << tag.name;

        if (!tag.aliases.empty()) {
            out
                << Key << "aliases"
                << Value << tag.aliases;
        }

        if (tag.description.has_value()) {
            const auto& description = tag.description.value();
            out << Key << "description" << Value;

            if (description.find("\n") != std::string::npos) {
                out << Literal;
            }

            out << description;
        }

        if (!tag.sources.empty()) {
            out << Key << "links" << Value << BeginSeq;

            for (const auto& source : tag.sources) {
                out << source.url;
            }

            out << EndSeq;
        }

        out
            << Key << "created"
            << Value << tag.date_created;

        out << EndMap;
        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::tag_preview& tag
    ) -> Emitter& {
        out
            << BeginMap
            << Key << "id" << Value << tag.id
            << Key << "name" << Value << tag.name
            << EndMap;

        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::object& object
    ) -> Emitter& {
        out << BeginMap
            << Key << "id" << Value << object.id
            << Key << "hash" << Value << object.hash
            << Key << "size" << Value << object.size
            << Key << "type" << Value << object.type
            << Key << "subtype" << Value << object.subtype
            << Key << "added" << Value << object.date_added;

        if (object.preview_id) {
            out << Key << "preview" << Value << *object.preview_id;
        }

        if (object.src) {
            out << Key << "source" << Value << object.src.value().url;
        }

        if (!object.posts.empty()) {
            out << Key << "posts" << Value << BeginSeq;
            for (const auto& post : object.posts) out << post;
            out << EndSeq;

        }

        out << EndMap;
        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::object_preview& object
    ) -> Emitter& {
        out << BeginMap
            << Key << "id" << Value << object.id
            << Key << "type" << Value << object.type
            << Key << "subtype" << Value << object.subtype
            << EndMap;

        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::post& post
    ) -> Emitter& {
        out << BeginMap;

        out << Key << "id" << Value << post.id;

        if (post.title) out << Key << "title" << Value << post.title.value();

        if (post.description)
            out << Key << "description" << Value << post.description.value();

        out << Key << "created" << Value << post.date_created;

        if (post.date_modified != post.date_created)
            out << Key << "modified" << Value << post.date_modified;

        if (!post.tags.empty()) {
            out << Key << "tags" << Value << BeginSeq;
            for (const auto& tag : post.tags) out << tag;
            out << EndSeq;
        }

        if (!post.objects.empty()) {
            out << "objects" << Value << BeginSeq;
            for (const auto& object : post.objects) out << object;
            out << EndSeq;
        }

        out << EndMap;
        return out;
    }

    auto operator<<(
        Emitter& out,
        const minty::core::post_preview& post
    ) -> Emitter& {
        out << BeginMap;

        out << Key << "id" << Value << post.id;
        if (post.title) out << Key << "title" << Value << post.title.value();
        out << Key << "objects" << Value << post.object_count;
        out << Key << "comments" << Value << post.comment_count;
        out << Key << "created" << Value << post.date_created;

        out << EndMap;

        return out;
    }
}
