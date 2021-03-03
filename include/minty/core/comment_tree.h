#pragma once

#include <minty/core/model.h>

#include <span>

namespace minty::core {
    auto build_tree(
        std::span<const repo::db::comment> entities
    ) -> comment_tree;
}
