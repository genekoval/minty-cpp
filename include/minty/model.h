#pragma once

#include <optional>
#include <string>

namespace minty::model {
    struct site {
        const std::string id;
        const std::string name;
        const std::string homepage;
        const std::optional<std::string> thumbnail_id;
    };

    struct tag {
        const std::string id;
        const std::string name;
        const std::string color;
        const std::string date_created;
    };
}
