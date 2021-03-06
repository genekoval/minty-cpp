#pragma once

#include <netcore/socket.h>
#include <zipline/zipline>

namespace minty::net {
    using event_t = unsigned int;
    using socket = netcore::socket;
    using data_stream = zipline::data_stream<socket>;

    template <typename T>
    using xfr = zipline::transfer<socket, T>;
}
