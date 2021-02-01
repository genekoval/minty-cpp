#pragma once

#include <minty/core/model.h>
#include <minty/server/server_info.h>

#include <zipline/zipline>

namespace zipline {
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
    struct transfer<Socket, minty::server::server_info> {
        using T = minty::server::server_info;
        using version_t = std::remove_const_t<decltype(T::version)>;

        static auto read(const Socket& sock) -> T {
            return {
                .version = transfer<Socket, version_t>::read(sock)
            };
        }

        static auto write(const Socket& sock, const T& t) -> void {
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
}
