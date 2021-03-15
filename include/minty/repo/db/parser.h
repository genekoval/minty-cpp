#pragma once

#include <minty/repo/db/db.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    template <>
    struct parser<comment> {
        static auto read(row_iterator& it, transaction& tx) -> comment {
            return {
                .id = read_field<decltype(comment::id)>(it),
                .post_id = read_field<decltype(comment::post_id)>(it),
                .parent_id = read_field<decltype(comment::parent_id)>(it),
                .indent = read_field<decltype(comment::indent)>(it),
                .content = read_field<decltype(comment::content)>(it),
                .date_created = read_field<decltype(comment::date_created)>(it)
            };
        };
    };

    template <>
    struct parser<creator> {
        static auto read(row_iterator& it, transaction& tx) -> creator {
            return {
                .id = read_field<decltype(creator::id)>(it),
                .name = read_field<decltype(creator::name)>(it),
                .aliases = read_array<decltype(creator::aliases)>(it),
                .bio = read_field<decltype(creator::bio)>(it),
                .avatar = read_field<decltype(creator::avatar)>(it),
                .banner = read_field<decltype(creator::banner)>(it),
                .sources = read_entities<decltype(creator::sources)>(
                    it, tx, "read_sources"
                ),
                .post_count = read_field<decltype(creator::post_count)>(it),
                .date_added = read_field<decltype(creator::date_added)>(it)
            };
        }
    };

    template <>
    struct parser<creator_preview> {
        static auto read(row_iterator& it, transaction& tx) -> creator_preview {
            return {
                .id = read_field<decltype(creator_preview::id)>(it),
                .name = read_field<decltype(creator_preview::name)>(it),
                .avatar = read_field<decltype(creator_preview::avatar)>(it)
            };
        }
    };

    template <>
    struct parser<object> {
        static auto read(row_iterator& it, transaction& tx) -> object {
            return {
                .id = read_field<decltype(object::id)>(it),
                .preview_id = read_field<decltype(object::preview_id)>(it),
                .src = read_entity<decltype(object::src)>(it, tx)
            };
        }
    };

    template <>
    struct parser<post> {
        static auto read(row_iterator& it, transaction& tx) -> post {
            return {
                .id = read_field<decltype(post::id)>(it),
                .description = read_field<decltype(post::description)>(it),
                .date_created = read_field<decltype(post::date_created)>(it),
                .date_modified = read_field<decltype(post::date_modified)>(it),
                .tags = read_entities<decltype(post::tags)>(
                    it, tx, "read_tags"
                ),
                .creators = read_entities<decltype(post::creators)>(
                    it, tx, "read_creator_previews"
                )
            };
        }
    };

    template <>
    struct parser<post_preview> {
        using T = post_preview;

        static auto read(row_iterator& it, transaction& tx) -> T {
            return {
                .id = read_field<decltype(T::id)>(it),
                .description = read_field<decltype(T::description)>(it),
                .preview_id = read_field<decltype(T::preview_id)>(it),
                .comment_count = read_field<decltype(T::comment_count)>(it),
                .object_count = read_field<decltype(T::object_count)>(it),
                .date_created = read_field<decltype(T::date_created)>(it)
            };
        }
    };

    template <>
    struct parser<site> {
        static auto read(row_iterator& it, transaction& tx) -> site {
            return {
                .id = read_field<decltype(site::id)>(it),
                .name = read_field<decltype(site::name)>(it),
                .homepage = read_field<decltype(site::homepage)>(it),
                .thumbnail_id = read_field<decltype(site::thumbnail_id)>(it)
            };
        }
    };

    template <>
    struct parser<source> {
        static auto read(row_iterator& it, transaction& tx) -> source {
            return {
                .id = read_field<decltype(source::id)>(it),
                .url = read_field<decltype(source::url)>(it),
                .website = read_entity<decltype(source::website)>(it, tx)
            };
        }
    };

    template <>
    struct parser<tag> {
        static auto read(row_iterator& it, transaction& tx) -> tag {
            return {
                .id = read_field<decltype(tag::id)>(it),
                .name = read_field<decltype(tag::name)>(it),
                .color = read_field<decltype(tag::color)>(it),
                .date_created = read_field<decltype(tag::date_created)>(it)
            };
        }
    };
}
