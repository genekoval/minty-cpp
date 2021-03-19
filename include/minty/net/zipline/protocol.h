#pragma once

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty::net {
    constexpr auto buffer_size = 8192;

    using event_t = unsigned int;

    using socket = zipline::buffered_socket<netcore::socket, buffer_size>;
    using data_stream = zipline::data_stream<socket>;

    template <typename T>
    using xfr = zipline::transfer<socket, T>;
}
