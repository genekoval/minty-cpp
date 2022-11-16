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

    template <typename Socket>
    struct coder<Socket, minty::core::comment_node> {
        static auto encode(
            Socket& socket,
            const minty::core::comment_node& node
        ) -> ext::task<> {
            co_await coder<Socket, minty::core::comment>::encode(
                socket,
                node.data
            );

            for (const auto* child : node.children) {
                co_await coder<Socket, minty::core::comment_node>::encode(
                    socket,
                    *child
                );
            }
        }
    };

    template <typename Socket>
    struct coder<Socket, minty::core::comment_tree> {
        static auto encode(
            Socket& socket,
            const minty::core::comment_tree& tree
        ) -> ext::task<> {
            co_await encode_size(socket, tree.total);

            for (const auto* root : tree.roots) {
                co_await coder<Socket, minty::core::comment_node>::encode(
                    socket,
                    *root
                );
            }
        }
    };

}
