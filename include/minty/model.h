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

    struct source {
        const std::string id;
        const site website;
        const std::string url;
    };

    struct object {
        const std::string id;
        const std::optional<std::string> preview_id;
        const std::optional<source> src;
    };

    struct tag {
        const std::string id;
        const std::string name;
        const std::string color;
        const std::string date_created;
    };
}
