#pragma once

#include "event.hpp"

#include <fstore/fstore>

namespace minty::net {
    using socket = fstore::net::socket;

    using error_list = zipline::error_list<>;

    using stream = zipline::stream<socket>;

    using client_type = zipline::client<socket, event>;
}
