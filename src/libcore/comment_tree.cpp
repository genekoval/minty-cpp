#include <minty/core/comment_tree.h>

#include <unordered_map>

namespace minty::core {
    comment_tree::comment_tree (
        std::span<const repo::db::comment> entities
    ) :
        total(entities.size()),
        comments(std::make_unique<comment_node[]>(total))
    {
        auto map = std::unordered_map<std::string, comment_node*>();
        comment_node* parent = nullptr;

        for (auto i = 0UL; i < entities.size(); ++i) {
            const auto& entity = entities[i];
            comments[i] = {{
                entity.id,
                entity.content,
                entity.indent,
                entity.date_created
            }};
            auto& comment = comments[i];
            map[comment.model.id] = &comment;

            if (!entity.parent_id) {
                m_roots.push_back(&comment);
                continue;
            }

            const auto& parent_id = entity.parent_id.value();

            if (!parent || parent_id != parent->model.id) {
                parent = map.at(parent_id);
            }

            parent->children.push_back(&comment);
        }
    }

    auto comment_tree::roots() const -> std::span<comment_node* const> {
        return std::span(
            m_roots.cbegin(),
            m_roots.cend()
        );
    }

    auto comment_tree::size() const -> std::size_t {
        return total;
    }
}
