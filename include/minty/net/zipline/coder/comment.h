#pragma once

#include <minty/core/model.h>

#include <zipline/zipline>

namespace zipline {
    ZIPLINE_OBJECT(
        minty::core::comment,
        &minty::core::comment::id,
        &minty::core::comment::content,
        &minty::core::comment::indent,
        &minty::core::comment::date_created
    );

    ZIPLINE_OBJECT(
        minty::core::comment_detail,
        &minty::core::comment_detail::id,
        &minty::core::comment_detail::post_id,
        &minty::core::comment_detail::parent_id,
        &minty::core::comment_detail::indent,
        &minty::core::comment_detail::content,
        &minty::core::comment_detail::date_created
    );

    template <io::writer Writer>
    struct encoder<minty::core::comment_node, Writer> {
        static auto encode(
            const minty::core::comment_node& node,
            Writer& writer
        ) -> ext::task<> {
            co_await zipline::encode(node.data, writer);

            for (const auto* child : node.children) {
                co_await zipline::encode(*child, writer);
            }
        }
    };

    template <io::writer Writer>
    struct encoder<minty::core::comment_tree, Writer> {
        static auto encode(
            const minty::core::comment_tree& tree,
            Writer& writer
        ) -> ext::task<> {
            co_await zipline::encode(tree.total, writer);

            for (const auto* root : tree.roots) {
                co_await zipline::encode(*root, writer);
            }
        }
    };
}
