#pragma once

#include <minty/repo/db/db.h>

#include <vector>

namespace minty::repo::db {
    struct site : entity<4> {
        const std::string id;
        const std::string name;
        const std::string homepage;
        const std::optional<std::string> thumbnail_id;

        site(row_iterator& it, transaction& tx);
    };

    struct source : entity<2, site> {
        const std::string id;
        const std::string url;
        const site website;

        source(row_iterator& it, transaction& tx);
    };

    struct object : entity<2, source> {
        const std::string id;
        const std::optional<std::string> preview_id;
        const std::optional<source> src;

        object(row_iterator& it, transaction& tx);
    };

    struct tag : entity<4> {
        const std::string id;
        const std::string name;
        const std::string color;
        const std::string date_created;

        tag(row_iterator& it, transaction& tx);
    };

    struct creator : entity<8> {
        const std::string id;
        const std::string name;
        const std::vector<std::string> aliases;
        const std::optional<std::string> bio;
        const std::optional<std::string> avatar;
        const std::optional<std::string> banner;
        const std::vector<source> sources;
        const std::string date_added;

        creator(row_iterator& it, transaction& tx);
    };

    struct creator_preview : entity<3> {
        const std::string id;
        const std::string name;
        const std::optional<std::string> avatar;

        creator_preview(row_iterator& it, transaction& tx);
    };

    struct post : entity<7> {
        const std::string id;
        const std::optional<std::string> description;
        const std::string date_created;
        const std::string date_modified;
        const std::vector<object> objects;
        const std::vector<tag> tags;
        const std::vector<creator_preview> creators;

        post(row_iterator& it, transaction& tx);
    };
}
