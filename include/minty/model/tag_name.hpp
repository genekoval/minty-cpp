#pragma once

#include <nlohmann/json.hpp>
#include <zipline/zipline>

namespace minty {
    struct tag_name {
        std::string name;
        std::vector<std::string> aliases;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(tag_name, name, aliases);
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::tag_name,
        &minty::tag_name::name,
        &minty::tag_name::aliases
    );
}
