#include <minty/core/comment_tree.h>

#include <unordered_map>

namespace minty::core {
    auto build_tree(
        std::span<const comment> entities
    ) -> comment_tree {
        auto tree = comment_tree {
            .total = entities.size(),
            .comments = std::make_unique<comment_node[]>(entities.size())
        };

        auto map = std::unordered_map<decltype(comment::id), comment_node*>();

        comment_node* parent = nullptr;

        for (auto i = 0UL; i < tree.total; ++i) {
            const auto& entity = entities[i];

            tree.comments[i] = {{
                entity.id,
                entity.content,
                entity.indent,
                entity.date_created
            }};

            auto& comment = tree.comments[i];
            map[comment.data.id] = &comment;

            if (!entity.parent_id) {
                tree.roots.push_back(&comment);
                continue;
            }

            const auto& parent_id = entity.parent_id.value();

            if (!parent || parent_id != parent->data.id) {
                parent = map.at(parent_id);
            }

            parent->children.push_back(&comment);
        }

        return tree;
    }
}
