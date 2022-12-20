#pragma once

#include <minty/model/comment.hpp>

#include <span>

namespace minty::core {
    auto build_tree(std::span<const comment> entities) -> comment_tree;
}
