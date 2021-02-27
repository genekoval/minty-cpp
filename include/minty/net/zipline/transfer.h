#pragma once

#include <minty/core/model.h>
#include <minty/core/comment_tree.h>
#include <minty/server/server_info.h>

#include <zipline/zipline>

namespace zipline {
    template <typename Socket>
    struct transfer<Socket, minty::core::comment> {
        using T = minty::core::comment;

        using id_t = decltype(T::id);
        using content_t = decltype(T::content);
        using indent_t = decltype(T::indent);
        using date_created_t = decltype(T::date_created);

        static auto read(const Socket& sock) -> T {
            return {
                .id = transfer<Socket, id_t>::read(sock),
                .content = transfer<Socket, content_t>::read(sock),
                .indent = transfer<Socket, indent_t>::read(sock),
                .date_created = transfer<Socket, date_created_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, id_t>::write(sock, t.id);
            transfer<Socket, content_t>::write(sock, t.content);
            transfer<Socket, indent_t>::write(sock, t.indent);
            transfer<Socket, date_created_t>::write(sock, t.date_created);
        }
    };

    template <typename Socket>
    auto write_tree(
        const Socket& sock,
        const minty::core::comment_node& root
    ) -> void {
        transfer<Socket, minty::core::comment>::write(sock, root.model);

        for (const auto* child : root.children) {
            write_tree(sock, *child);
        }
    }

    template <typename Socket>
    struct transfer<Socket, minty::core::comment_tree> {
        using T = minty::core::comment_tree;

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, std::size_t>::write(sock, t.size());
            const auto roots = t.roots();

            for (const auto* root : roots) {
                write_tree(sock, *root);
            }
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::creator> {
        using T = minty::core::creator;

        using id_t = decltype(T::id);
        using name_t = decltype(T::name);
        using aliases_t = decltype(T::aliases);
        using bio_t = decltype(T::bio);
        using avatar_t = decltype(T::avatar);
        using banner_t = decltype(T::banner);
        using sources_t = decltype(T::sources);
        using post_cout_t = decltype(T::post_count);
        using date_added_t = decltype(T::date_added);

        static auto read(const Socket& sock) -> minty::core::creator {
            return {
                .id = transfer<Socket, id_t>::read(sock),
                .name= transfer<Socket, name_t>::read(sock),
                .aliases = transfer<Socket, aliases_t>::read(sock),
                .bio = transfer<Socket, bio_t>::read(sock),
                .avatar = transfer<Socket, avatar_t>::read(sock),
                .banner = transfer<Socket, banner_t>::read(sock),
                .sources = transfer<Socket, sources_t>::read(sock),
                .post_count = transfer<Socket, post_cout_t>::read(sock),
                .date_added = transfer<Socket, date_added_t>::read(sock)
            };
        }

        static auto write(
            const Socket& sock,
            const minty::core::creator& t
        ) -> void {
            transfer<Socket, id_t>::write(sock, t.id);
            transfer<Socket, name_t>::write(sock, t.name);
            transfer<Socket, aliases_t>::write(sock, t.aliases);
            transfer<Socket, bio_t>::write(sock, t.bio);
            transfer<Socket, avatar_t>::write(sock, t.avatar);
            transfer<Socket, banner_t>::write(sock, t.banner);
            transfer<Socket, sources_t>::write(sock, t.sources);
            transfer<Socket, post_cout_t>::write(sock, t.post_count);
            transfer<Socket, date_added_t>::write(sock, t.date_added);
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::creator_preview> {
        using T = minty::core::creator_preview;

        using id_t = decltype(T::id);
        using name_t = decltype(T::name);
        using avatar_t = decltype(T::avatar);

        static auto read(const Socket& sock) -> T {
            return {
                .id = transfer<Socket, id_t>::read(sock),
                .name = transfer<Socket, name_t>::read(sock),
                .avatar = transfer<Socket, avatar_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, id_t>::write(sock, t.id);
            transfer<Socket, name_t>::write(sock, t.name);
            transfer<Socket, avatar_t>::write(sock, t.avatar);
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::object> {
        using T = minty::core::object;

        using id_t = decltype(T::id);
        using hash_t = decltype(T::hash);
        using size_t = decltype(T::size);
        using mime_type_t = decltype(T::mime_type);
        using date_added_t = decltype(T::date_added);
        using preview_id_t = decltype(T::preview_id);
        using src_t = decltype(T::src);

        static auto read(const Socket& sock) -> T {
            return {
                .id = transfer<Socket, id_t>::read(sock),
                .hash = transfer<Socket, hash_t>::read(sock),
                .size = transfer<Socket, size_t>::read(sock),
                .mime_type = transfer<Socket, mime_type_t>::read(sock),
                .date_added = transfer<Socket, date_added_t>::read(sock),
                .preview_id = transfer<Socket, preview_id_t>::read(sock),
                .src = transfer<Socket, src_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, id_t>::write(sock, t.id);
            transfer<Socket, hash_t>::write(sock, t.hash);
            transfer<Socket, size_t>::write(sock, t.size);
            transfer<Socket, mime_type_t>::write(sock, t.mime_type);
            transfer<Socket, date_added_t>::write(sock, t.date_added);
            transfer<Socket, preview_id_t>::write(sock, t.preview_id);
            transfer<Socket, src_t>::write(sock, t.src);
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::post> {
        using T = minty::core::post;

        using id_t = decltype(T::id);
        using description_t = decltype(T::description);
        using date_created_t = decltype(T::date_created);
        using date_modified_t = decltype(T::date_modified);
        using objects_t = decltype(T::objects);
        using tags_t = decltype(T::tags);
        using creators_t = decltype(T::creators);

        static auto read(const Socket& sock) -> T {
            return {
                .id = transfer<Socket, id_t>::read(sock),
                .description = transfer<Socket, description_t>::read(sock),
                .date_created = transfer<Socket, date_created_t>::read(sock),
                .date_modified = transfer<Socket, date_modified_t>::read(sock),
                .objects = transfer<Socket, objects_t>::read(sock),
                .tags = transfer<Socket, tags_t>::read(sock),
                .creators = transfer<Socket, creators_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, id_t>::write(sock, t.id);
            transfer<Socket, description_t>::write(sock, t.description);
            transfer<Socket, date_created_t>::write(sock, t.date_created);
            transfer<Socket, date_modified_t>::write(sock, t.date_modified);
            transfer<Socket, objects_t>::write(sock, t.objects);
            transfer<Socket, tags_t>::write(sock, t.tags);
            transfer<Socket, creators_t>::write(sock, t.creators);
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::post_preview> {
        using T = minty::core::post_preview;

        using id_t = decltype(T::id);
        using description_t = decltype(T::description);
        using date_created_t = decltype(T::date_created);
        using date_modified_t = decltype(T::date_modified);

        static auto read(const Socket& sock) -> T {
            return {
                .id = transfer<Socket, id_t>::read(sock),
                .description = transfer<Socket, description_t>::read(sock),
                .date_created = transfer<Socket, date_created_t>::read(sock),
                .date_modified = transfer<Socket, date_modified_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, id_t>::write(sock, t.id);
            transfer<Socket, description_t>::write(sock, t.description);
            transfer<Socket, date_created_t>::write(sock, t.date_created);
            transfer<Socket, date_modified_t>::write(sock, t.date_modified);
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::server::server_info> {
        using T = minty::server::server_info;

        using object_source_t = std::remove_const_t<decltype(T::object_source)>;
        using version_t = std::remove_const_t<decltype(T::version)>;

        static auto read(const Socket& sock) -> T {
            return {
                .object_source = transfer<Socket, object_source_t>::read(sock),
                .version = transfer<Socket, version_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, object_source_t>::write(sock, t.object_source);
            transfer<Socket, version_t>::write(sock, t.version);
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::site> {
        static auto read(const Socket& sock) -> minty::core::site {
            return {
                .id = transfer<Socket,
                    decltype(minty::core::site::id)>::read(sock),
                .name = transfer<Socket,
                    decltype(minty::core::site::name)>::read(sock),
                .homepage = transfer<Socket,
                    decltype(minty::core::site::homepage)>::read(sock),
                .thumbnail_id = transfer<Socket,
                    decltype(minty::core::site::thumbnail_id)>::read(sock)
            };
        }

        static auto write(
            const Socket& sock,
            const minty::core::site& site
        ) -> void {
            transfer<Socket, decltype(minty::core::site::id)>::write(
                sock,
                site.id
            );

            transfer<Socket, decltype(minty::core::site::name)>::write(
                sock,
                site.name
            );

            transfer<Socket, decltype(minty::core::site::homepage)>::write(
                sock,
                site.homepage
            );

            transfer<Socket, decltype(minty::core::site::thumbnail_id)>::write(
                sock,
                site.thumbnail_id
            );
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::source> {
        static auto read(const Socket& sock) -> minty::core::source {
            return {
                .id = transfer<Socket,
                    decltype(minty::core::source::id)>::read(sock),
                .url= transfer<Socket,
                    decltype(minty::core::source::url)>::read(sock),
                .website= transfer<Socket,
                    decltype(minty::core::source::website)>::read(sock)
            };
        }

        static auto write(
            const Socket& sock,
            const minty::core::source& source
        ) -> void {
            transfer<Socket, decltype(minty::core::source::id)>::write(
                sock,
                source.id
            );

            transfer<Socket, decltype(minty::core::source::url)>::write(
                sock,
                source.url
            );

            transfer<Socket, decltype(minty::core::source::website)>::write(
                sock,
                source.website
            );
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::tag> {
        using T = minty::core::tag;

        using id_t = decltype(T::id);
        using name_t = decltype(T::name);
        using color_t = decltype(T::color);
        using date_created_t = decltype(T::date_created);

        static auto read(const Socket& sock) -> T {
            return {
                .id = transfer<Socket, id_t>::read(sock),
                .name = transfer<Socket, name_t>::read(sock),
                .color = transfer<Socket, color_t>::read(sock),
                .date_created = transfer<Socket, date_created_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
            transfer<Socket, id_t>::write(sock, t.id);
            transfer<Socket, name_t>::write(sock, t.name);
            transfer<Socket, color_t>::write(sock, t.color);
            transfer<Socket, date_created_t>::write(sock, t.date_created);
        }
    };
}
