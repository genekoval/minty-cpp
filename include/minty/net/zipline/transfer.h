#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    template <typename Socket>
    struct transfer<Socket, minty::core::creator> {
        static auto read(const Socket& sock) -> minty::core::creator {
            return {
                .id = transfer<Socket,
                    decltype(minty::core::creator::id)>::read(sock),
                .name= transfer<Socket,
                    decltype(minty::core::creator::name)>::read(sock),
                .aliases = transfer<Socket,
                    decltype(minty::core::creator::aliases)>::read(sock),
                .bio = transfer<Socket,
                    decltype(minty::core::creator::bio)>::read(sock),
                .avatar = transfer<Socket,
                    decltype(minty::core::creator::avatar)>::read(sock),
                .banner = transfer<Socket,
                    decltype(minty::core::creator::banner)>::read(sock),
                .sources = transfer<Socket,
                    decltype(minty::core::creator::sources)>::read(sock),
                .date_added = transfer<Socket,
                    decltype(minty::core::creator::date_added)>::read(sock)
            };
        }

        static auto write(
            const Socket& sock,
            const minty::core::creator& creator
        ) -> void {
            transfer<Socket, decltype(minty::core::creator::id)>::write(
                sock,
                creator.id
            );

            transfer<Socket, decltype(minty::core::creator::name)>::write(
                sock,
                creator.name
            );

            transfer<Socket, decltype(minty::core::creator::aliases)>::write(
                sock,
                creator.aliases
            );

            transfer<Socket, decltype(minty::core::creator::bio)>::write(
                sock,
                creator.bio
            );

            transfer<Socket, decltype(minty::core::creator::avatar)>::write(
                sock,
                creator.avatar
            );

            transfer<Socket, decltype(minty::core::creator::banner)>::write(
                sock,
                creator.banner
            );

            transfer<Socket, decltype(minty::core::creator::sources)>::write(
                sock,
                creator.sources
            );

            transfer<Socket, decltype(minty::core::creator::date_added)>::write(
                sock,
                creator.date_added
            );
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
