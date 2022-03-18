#pragma once

#include <minty/repo/db/model.h>

#include <fstore/client.h>
#include <memory>

namespace minty::core {
    using range = repo::db::range;
    using post_search = repo::db::post_search;
    using post_update = repo::db::post_update;
    using tag_name = repo::db::tag_name;
    using tag_preview = repo::db::tag_preview;
    using tag_text = repo::db::tag_text;

    enum class sort_order : uint8_t {
        ascending,
        descending
    };

    enum class post_sort_value : uint8_t {
        date_created,
        date_modified,
        relevance,
        title
    };

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

    template <typename T>
    struct modification {
        std::string date_modified;
        T new_value;
    };

    struct object_preview {
        decltype(repo::db::object_preview::id) id;
        decltype(repo::db::object_preview::preview_id) preview_id;
        decltype(fstore::object_meta::type) type;
        decltype(fstore::object_meta::subtype) subtype;

        object_preview() = default;
        object_preview(
            repo::db::object_preview&& obj,
            fstore::object_meta&& meta
        );
        object_preview(
            std::string&& id,
            std::optional<std::string>&& preview_id,
            std::string&& type,
            std::string&& subtype
        );
    };

    struct post {
        std::string id;
        std::optional<std::string> title;
        std::optional<std::string> description;
        std::string date_created;
        std::string date_modified;
        std::vector<object_preview> objects;
        std::vector<tag_preview> tags;
    };

    struct post_parts {
        std::optional<std::string> title;
        std::optional<std::string> description;
        std::vector<std::string> objects;
        std::vector<std::string> tags;
    };

    struct post_preview {
        decltype(repo::db::post_preview::id) id;
        decltype(repo::db::post_preview::title) title;
        std::optional<object_preview> preview;
        decltype(repo::db::post_preview::comment_count) comment_count;
        decltype(repo::db::post_preview::object_count) object_count;
        decltype(repo::db::post_preview::date_created) date_created;

        post_preview() = default;
        post_preview(
            repo::db::post_preview&& p,
            std::optional<object_preview>&& obj
        );
    };

    struct object {
        std::string id;
        std::string hash;
        data_size size;
        std::string type;
        std::string subtype;
        std::string date_added;
        std::optional<std::string> preview_id;
        std::optional<source> src;
        std::vector<post_preview> posts;

        object() = default;
        object(
            repo::db::object&& obj,
            fstore::object_meta&& meta,
            std::vector<post_preview>&& posts
        );
    };

    struct post_query {
        struct sort_type {
            sort_order order;
            post_sort_value value;
        };

        unsigned int from;
        unsigned int size;
        std::optional<std::string> text;
        std::vector<std::string> tags;
        sort_type sort;
    };

    template <typename T>
    struct search_result {
        unsigned int total;
        std::vector<T> hits;
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

    struct tag_query {
        unsigned int from;
        unsigned int size;
        std::string name;
        std::vector<std::string> exclude;
    };
}
