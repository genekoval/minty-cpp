#pragma once

#include <minty/repo/db/model.h>

#include <fstore/client.h>
#include <memory>

namespace minty::core {
    using post_preview = repo::db::post_preview;
    using tag_name = repo::db::tag_name;
    using tag_preview = repo::db::tag_preview;

    struct source {
        decltype(repo::db::source::id) id;
        std::string url;
        decltype(repo::db::site::icon) icon;

        source() = default;
        source(const repo::db::source& src);
    };

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

    struct data_size {
        uintmax_t bytes;
        std::string formatted;

        data_size() = default;
        data_size(uintmax_t bytes);
    };

    struct object {
        std::string id;
        std::string hash;
        data_size size;
        std::string mime_type;
        std::string date_added;
        std::optional<std::string> preview_id;
        std::optional<source> src;

        object() = default;
        object(
            const repo::db::object& obj,
            const fstore::object_meta& meta
        );
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

    struct tag {
        decltype(repo::db::tag::id) id;
        decltype(repo::db::tag::name) name;
        decltype(repo::db::tag::aliases) aliases;
        decltype(repo::db::tag::description) description;
        decltype(repo::db::tag::avatar) avatar;
        decltype(repo::db::tag::banner) banner;
        std::vector<source> sources;
        decltype(repo::db::tag::post_count) post_count;
        decltype(repo::db::tag::date_created) date_created;

        tag() = default;
        tag(
            const repo::db::tag& t,
            std::vector<repo::db::source>&& sources
        );
    };
}
