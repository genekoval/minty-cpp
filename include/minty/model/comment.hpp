#pragma once

#include "time_point.hpp"

#include <ext/json.hpp>
#include <memory>
#include <uuid++/json.hpp>
#include <uuid++/uuid++>
#include <vector>
#include <zipline/zipline>

namespace minty {
    struct comment {
        UUID::uuid id;
        UUID::uuid post_id;
        std::optional<decltype(id)> parent_id;
        std::int16_t indent;
        std::string content;
        time_point date_created;

        auto operator==(const comment&) const noexcept -> bool = default;
    };

    struct comment_data {
        decltype(comment::id) id;
        decltype(comment::content) content;
        decltype(comment::indent) indent;
        decltype(comment::date_created) date_created;

        auto operator==(const comment_data&) const noexcept -> bool = default;
    };

    struct comment_node {
        comment_data data;
        std::vector<comment_node*> children;
    };

    struct comment_tree {
        std::size_t total;
        std::unique_ptr<comment_node[]> comments;
        std::vector<comment_node*> roots;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        comment,
        id,
        post_id,
        parent_id,
        indent,
        content,
        date_created
    );

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        comment_data,
        id,
        content,
        indent,
        date_created
    );

    auto to_json(nlohmann::json& j, const comment_node& node) -> void;

    auto to_json(nlohmann::json& j, const comment_tree& tree) -> void;
}

namespace zipline {
    ZIPLINE_OBJECT(
        minty::comment,
        &minty::comment::id,
        &minty::comment::post_id,
        &minty::comment::parent_id,
        &minty::comment::indent,
        &minty::comment::content,
        &minty::comment::date_created
    );

    ZIPLINE_OBJECT(
        minty::comment_data,
        &minty::comment_data::id,
        &minty::comment_data::content,
        &minty::comment_data::indent,
        &minty::comment_data::date_created
    );

    template <io::writer Writer>
    struct encoder<minty::comment_node, Writer> {
        static auto encode(
            const minty::comment_node& comment,
            Writer& writer
        ) -> ext::task<> {
            co_await zipline::encode(comment.data, writer);

            for (const auto* child : comment.children) {
                co_await zipline::encode(*child, writer);
            }
        }
    };

    template <io::writer Writer>
    struct encoder<minty::comment_tree, Writer> {
        static auto encode(
            const minty::comment_tree& tree,
            Writer& writer
        ) -> ext::task<> {
            co_await zipline::encode(tree.total, writer);

            for (const auto* root : tree.roots) {
                co_await zipline::encode(*root, writer);
            }
        }
    };
}
