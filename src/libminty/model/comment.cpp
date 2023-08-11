#include <minty/model/comment.hpp>

using json = nlohmann::json;

namespace minty {
    auto to_json(json& j, const comment_node& node) -> void {
        j.push_back(node.data);

        for (const auto* const child : node.children) {
            to_json(j, *child);
        }
    }

    auto to_json(json& j, const comment_tree& tree) -> void {
        j = json::array();

        for (const auto* const root : tree.roots) {
            to_json(j, *root);
        }
    }
}
