#include <minty/core/search.h>
#include <minty/net/zipline/transfer.h>

#include <ext/except.h>

namespace minty::core {
    search_engine::search_engine(std::string_view endpoint) :
        endpoint(endpoint)
    {
        INFO() << "Using search engine version " << version();
    }

    auto search_engine::connect() -> client {
        try {
            return client(errors, net::socket(netcore::connect(endpoint)));
        }
        catch (const ext::system_error& ex) {
            ERROR() << ex.what();
            throw std::runtime_error("search service unavailable");
        }
    }

    auto search_engine::add_post(const post_search& post) -> void {
        connect().send<void>(event::add_post, post);
    }

    auto search_engine::add_tags(std::span<const tag_text> tags) -> void {
        if (tags.empty()) return;

        connect().send<void>(
            event::add_tags,
            tags
        );
    }

    auto search_engine::add_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> void {
        connect().send<void>(
            event::add_tag_alias,
            tag_id,
            alias
        );
    }

    auto search_engine::create_indices() -> void {
        connect().send<void>(event::create_indices);
    }

    auto search_engine::delete_indices() -> void {
        connect().send<void>(event::delete_indices);
    }

    auto search_engine::delete_post(std::string_view post_id) -> void {
        connect().send<void>(event::delete_post, post_id);
    }

    auto search_engine::delete_tag(std::string_view tag_id) -> void {
        connect().send<void>(
            event::delete_tag,
            tag_id
        );
    }

    auto search_engine::delete_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> void {
        connect().send<void>(
            event::delete_tag_alias,
            tag_id,
            alias
        );
    }

    auto search_engine::find_post(
        std::string_view term,
        std::span<std::string> tags
    ) -> std::vector<std::string> {
        return connect().send<std::vector<std::string>>(
            event::find_post,
            term,
            tags
        );
    }

    auto search_engine::find_tags_by_name(
        std::string_view term
    ) -> std::vector<std::string> {
        return connect().send<std::vector<std::string>>(
            event::find_tags_by_name,
            term
        );
    }

    auto search_engine::update_tag_name(
        std::string_view tag_id,
        std::string_view old_name,
        std::string_view new_name
    ) -> void {
        connect().send<void>(
            event::update_tag_name,
            tag_id,
            old_name,
            new_name
        );
    }

    auto search_engine::version() -> std::string {
        return connect().send<std::string>(event::version);
    }
}
