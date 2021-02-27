#pragma once

#include <minty/core/model.h>

#include <memory>
#include <span>

namespace minty::core {
    struct comment_node {
        comment model;
        std::vector<comment_node*> children;
    };

    class comment_tree {
        const std::size_t total;
        const std::unique_ptr<comment_node[]> comments;
        std::vector<comment_node*> m_roots;

        auto add(comment&& c) -> comment&;
    public:
        comment_tree(std::span<const repo::db::comment> entities);

        auto roots() const -> std::span<comment_node* const>;

        auto size() const -> std::size_t;
    };
}
