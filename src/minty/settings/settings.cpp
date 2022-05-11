#include "yaml.h"

namespace minty::cli {
    auto settings::load(std::string_view text) -> settings {
        return YAML::Load(text.data()).as<settings>();
    }

    auto settings::load(const std::filesystem::path& path) -> settings {
        return YAML::LoadFile(path.c_str()).as<settings>();
    }
}
