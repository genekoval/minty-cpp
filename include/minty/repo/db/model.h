#pragma once

#include <minty/repo/db/db.h>

namespace minty::repo::db {
    struct site : entity<4> {
        const std::string id;
        const std::string name;
        const std::string homepage;
        const std::optional<std::string> thumbnail_id;

        site(row_iterator& it);
    };

    struct source : entity<2, site> {
        const std::string id;
        const std::string url;
        const site website;

        source(row_iterator& it);
    };

    struct object : entity<2, source> {
        const std::string id;
        const std::optional<std::string> preview_id;
        const std::optional<source> src;

        object(row_iterator& it);
    };

    struct tag : entity<4> {
        const std::string id;
        const std::string name;
        const std::string color;
        const std::string date_created;

        tag(row_iterator& it);
    };
}
