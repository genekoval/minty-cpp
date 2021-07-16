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

    template <typename Socket>
    struct transfer<Socket, minty::core::comment_node> {
        static auto write(
            Socket& socket,
            const minty::core::comment_node& node
        ) -> void {
            transfer<Socket, minty::core::comment>::write(socket, node.data);

            for (const auto* child : node.children) {
                transfer<Socket, minty::core::comment_node>::write(
                    socket,
                    *child
                );
            }
        }
    };

    template <typename Socket>
    struct transfer<Socket, minty::core::comment_tree> {
        static auto write(
            Socket& socket,
            const minty::core::comment_tree& tree
        ) -> void {
            write_size(socket, tree.total);

            for (const auto* root : tree.roots) {
                transfer<Socket, minty::core::comment_node>::write(
                    socket,
                    *root
                );
            }
        }
    };

}
