#pragma once

#include <minty/repo/db/model.h>

#include <memory>

namespace minty::core {
    using post_preview = repo::db::post_preview;
    using site = repo::db::site;
    using source = repo::db::source;
    using tag = repo::db::tag;
    using tag_name = repo::db::tag_name;
    using tag_preview = repo::db::tag_preview;

    struct comment {
        std::string id;
        std::string content;
        unsigned int indent;
        std::string date_created;
    };

    struct comment_node {
        comment data;
        std::vector<comment_node*> children;
    };

    struct comment_tree {
        const std::size_t total;
        std::unique_ptr<comment_node[]> comments;
        std::vector<comment_node*> roots;
    };

    struct object {
        std::string id;
        std::string hash;
        uintmax_t size;
        std::string mime_type;
        std::string date_added;
        std::optional<std::string> preview_id;
        std::optional<source> src;
    };

    struct post {
        std::string id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        std::string date_created;
        std::string date_modified;
        std::vector<object> objects;
        std::vector<tag_preview> tags;
    };

    struct post_parts {
        std::string title;
        std::string description;
        std::vector<std::string> objects;
        std::vector<std::string> tags;
    };
}
