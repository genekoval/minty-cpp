#include <minty/core/search.h>
#include <minty/net/zipline/transfer.h>

#include <ext/except.h>

namespace minty::core {
    search_engine::search_engine(std::string_view endpoint) :
        endpoint(endpoint)
    {
        TIMBER_DEBUG("Using search engine version {}", version());
    }

    auto search_engine::connect() -> client {
        try {
            return client(errors, net::socket(netcore::connect(endpoint)));
        }
        catch (const ext::system_error& ex) {
            TIMBER_ERROR("search search unavailable: {}", ex.what());
            throw std::runtime_error("search service unavailable");
        }
    }

    auto search_engine::add_post(const post_search& post) -> void {
        connect().send<void>(event::add_post, post);
    }

    auto search_engine::add_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        return connect().send<void>(event::add_post_tag, post_id, tag_id);
    }

    auto search_engine::add_posts(std::span<const post_search> posts) -> void {
        if (posts.empty()) return;
        connect().send<void>(event::add_posts, posts);
    }

    auto search_engine::add_tags(std::span<const tag_text> tags) -> void {
        if (tags.empty()) return;
        connect().send<void>(event::add_tags, tags);
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

    auto search_engine::find_posts(
        const post_query& query
    ) -> search_result<std::string> {
        return connect().send<search_result<std::string>>(
            event::find_posts,
            query
        );
    }

    auto search_engine::find_tags(
        const tag_query& query
    ) -> search_result<std::string> {
        return connect().send<search_result<std::string>>(
            event::find_tags,
            query
        );
    }

    auto search_engine::remove_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        connect().send<void>(event::remove_post_tag, post_id, tag_id);
    }

    auto search_engine::update_post_date_modified(
        std::string_view post_id,
        std::string_view date_modified
    ) -> void {
        connect().send<void>(
            event::update_post_date_modified,
            post_update {
                .id = std::string(post_id),
                .date_modified = std::string(date_modified)
            }
        );
    }

    auto search_engine::update_post_description(
        const post_update& post
    ) -> void {
        connect().send<void>(event::update_post_description, post);
    }

    auto search_engine::update_post_title(
        const post_update& post
    ) -> void {
        connect().send<void>(event::update_post_title, post);
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
