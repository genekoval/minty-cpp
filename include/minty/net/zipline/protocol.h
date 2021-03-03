#pragma once

#include <netcore/socket.h>
#include <zipline/zipline>

namespace minty::net {
    using event_t = unsigned int;
    using socket = netcore::socket;

    template <typename T>
    using xfr = zipline::transfer<socket, T>;
}
