#pragma once

#include <internal/conf/settings.hpp>

#include <conftools/yaml.h>

namespace YAML {
    CONVERT(minty::conf::settings);
    CONVERT(minty::conf::settings::s_daemon);
    CONVERT(minty::conf::settings::s_database);
    CONVERT(minty::conf::settings::s_fstore);
    CONVERT(minty::conf::settings::s_fstore::s_proxy);
    CONVERT(minty::conf::settings::s_log);
    CONVERT(minty::conf::settings::s_search);
}
