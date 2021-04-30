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
    struct parser<tag> {
        static auto read(row_iterator& it, transaction& tx) -> tag {
            return {
                .id = read_field<decltype(tag::id)>(it),
                .name = read_field<decltype(tag::name)>(it),
                .aliases = read_array<decltype(tag::aliases)>(it),
                .description = read_field<decltype(tag::description)>(it),
                .avatar = read_field<decltype(tag::avatar)>(it),
                .banner = read_field<decltype(tag::banner)>(it),
                .sources = read_entities<decltype(tag::sources)>(
                    it, tx, "read_sources"
                ),
                .post_count = read_field<decltype(tag::post_count)>(it),
                .date_created = read_field<decltype(tag::date_created)>(it)
            };
        }
    };

    template <>
    struct parser<tag_name> {
        static auto read(row_iterator& it, transaction& tx) -> tag_name {
            return {
                .name = read_field<decltype(tag_name::name)>(it),
                .aliases = read_array<decltype(tag_name::aliases)>(it)
            };
        }
    };

    template <>
    struct parser<tag_preview> {
        static auto read(row_iterator& it, transaction& tx) -> tag_preview {
            return {
                .id = read_field<decltype(tag_preview::id)>(it),
                .name = read_field<decltype(tag_preview::name)>(it),
                .avatar = read_field<decltype(tag_preview::avatar)>(it)
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
                .title = read_field<decltype(post::title)>(it),
                .description = read_field<decltype(post::description)>(it),
                .date_created = read_field<decltype(post::date_created)>(it),
                .date_modified = read_field<decltype(post::date_modified)>(it)
            };
        }
    };

    template <>
    struct parser<post_preview> {
        using T = post_preview;

        static auto read(row_iterator& it, transaction& tx) -> T {
            return {
                .id = read_field<decltype(T::id)>(it),
                .title = read_field<decltype(post::title)>(it),
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
                .scheme = read_field<decltype(site::scheme)>(it),
                .host = read_field<decltype(site::host)>(it),
                .icon = read_field<decltype(site::icon)>(it)
            };
        }
    };

    template <>
    struct parser<source> {
        static auto read(row_iterator& it, transaction& tx) -> source {
            return {
                .id = read_field<decltype(source::id)>(it),
                .resource = read_field<decltype(source::resource)>(it),
                .website = read_entity<decltype(source::website)>(it, tx)
            };
        }
    };
}
