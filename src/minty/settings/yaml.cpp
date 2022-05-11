#include "yaml.h"

namespace c = conftools;

using namespace minty::cli;

namespace YAML {
    DECODE(
        conf::log,
        c::optional("level", &conf::log::level)
    );

    DECODE(
        conf::server,
        c::required("objects", &conf::server::objects),
        c::required("path", &conf::server::path)
    );

    DECODE(
        settings,
        c::optional("log", &settings::log),
        c::required("server", &settings::server)
    );
}
