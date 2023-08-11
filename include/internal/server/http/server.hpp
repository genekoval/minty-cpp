#pragma once

#include <internal/core/repo.hpp>

#include <minty/model/server_info.hpp>

#include <conftools/endpoint.hpp>

namespace minty::server::http {
    auto listen(
        ::http::server::router& router,
        std::string_view cert,
        std::string_view key,
        std::span<const conftools::endpoint> endpoints
    ) -> ext::task<::http::server::server_list>;

    auto router(
        const server_info& info,
        core::repo& repo
    ) -> ::http::server::router;
}
