#include <minty/net/zipline/transfer/server_info.h>

namespace n = minty::net;
namespace s = minty::server;

namespace zipline {
    auto n::xfr<s::server_info>::read(n::socket& sock) -> type {
        return {
            .object_source = n::xfr<object_source_t>::read(sock),
            .version = n::xfr<version_t>::read(sock)
        };
    }

    auto n::xfr<s::server_info>::write(
        n::socket& sock,
        const type& t
    ) -> void {
        n::xfr<object_source_t>::write(sock, t.object_source);
        n::xfr<version_t>::write(sock, t.version);
    }
}
