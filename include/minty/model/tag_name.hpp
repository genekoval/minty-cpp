#pragma once

#include <zipline/zipline>

namespace minty {
    struct tag_name {
        std::string name;
        std::vector<std::string> aliases;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::tag_name,
        &minty::tag_name::name,
        &minty::tag_name::aliases
    );
}
