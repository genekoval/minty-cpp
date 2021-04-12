#pragma once

#include <minty/net/zipline/protocol.h>

#include <netcore/netcore>
#include <zipline/zipline>

namespace minty::core {
    namespace search {
        struct tag {
            std::string id;
            std::vector<std::string> names;
            std::optional<std::string> description;
        };
    };

    class search_engine {
        enum class event : net::event_t {
            add_tag,
            delete_tag,
            find_tags_by_name,
            version
        };

        using protocol = zipline::protocol<net::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;

        auto connect() -> client;

        auto version() -> std::string;
    public:
        search_engine(std::string_view endpoint);

        auto add_tag(const search::tag& tag) -> void;

        auto find_tags_by_name(
            std::string_view term
        ) -> std::vector<std::string>;

        auto delete_tag(std::string_view tag_id) -> void;
    };
}
