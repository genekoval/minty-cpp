#include "yaml.hpp"

#include <internal/conf/settings.hpp>

#include <minty/except.hpp>

namespace minty::conf {
    auto settings::load(std::string_view text) -> settings {
        return YAML::Load(text.data()).as<settings>();
    }

    auto settings::load_file(std::string_view path) -> settings {
        try {
            return YAML::LoadFile(path.data()).as<settings>();
        }
        catch (const YAML::BadFile& ex) {
            throw std::runtime_error(fmt::format(R"("{}": bad file)", path));
        }
        catch (const YAML::Exception& ex) {
            throw std::runtime_error(fmt::format(
                R"("{}": error at line {:L}, column {:L}: {})",
                path,
                ex.mark.line,
                ex.mark.column,
                ex.msg
            ));
        }
    }

    auto initialize(std::string_view path) -> settings {
        auto config = settings::load_file(path);

        timber::reporting_level = config.log.level;

        return config;
    }
}
