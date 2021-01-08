#pragma once

#include <minty/repo/db/model.h>

namespace minty::core {
    using creator = repo::db::creator;
    using creator_preview = repo::db::creator_preview;
    using source = repo::db::source;
    using tag = repo::db::tag;

    struct object {
        const std::string id;
        const std::string hash;
        const uintmax_t size;
        const std::string mime_type;
        const std::string date_added;
        const std::optional<std::string> preview_id;
        const std::optional<source> src;
    };

    struct post {
        const std::string id;
        const std::optional<std::string> description;
        const std::string date_created;
        const std::string date_modified;
        const std::vector<object> objects;
        const std::vector<tag> tags;
        const std::vector<creator_preview> creators;
    };
}
