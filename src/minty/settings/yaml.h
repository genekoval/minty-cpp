#pragma once

#include "settings.h"

#include <conftools/yaml.h>

namespace YAML {
    CONVERT(minty::cli::conf::log);
    CONVERT(minty::cli::conf::server);
    CONVERT(minty::cli::settings);
}
