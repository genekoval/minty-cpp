#pragma once

#include <internal/core/repo.hpp>
#include <minty/repo.hpp>

namespace minty::server {
    using std::chrono::seconds;
    using stream = zipline::stream<socket>;

    class router_context {
        core::repo* const repo;
        const server_info* const info;
    public:
        router_context(core::repo& repo, const server_info& info);

        auto add_comment(UUID::uuid post_id, std::string content)
            -> ext::task<comment_data>;

        auto add_object_data(stream stream) -> ext::task<object_preview>;

        auto add_objects_url(std::string url)
            -> ext::task<std::vector<object_preview>>;

        auto add_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects,
            std::optional<UUID::uuid> destination
        ) -> ext::task<time_point>;

        auto add_post_tag(UUID::uuid post_id, UUID::uuid tag_id) -> ext::task<>;

        auto add_related_post(UUID::uuid post_id, UUID::uuid related)
            -> ext::task<>;

        auto add_reply(UUID::uuid parent_id, std::string content)
            -> ext::task<comment_data>;

        auto add_tag(std::string name) -> ext::task<UUID::uuid>;

        auto add_tag_alias(UUID::uuid tag_id, std::string alias)
            -> ext::task<tag_name>;

        auto add_tag_source(UUID::uuid tag_id, std::string url)
            -> ext::task<source>;

        auto create_post(UUID::uuid post_id) -> ext::task<>;

        auto create_post_draft() -> ext::task<UUID::uuid>;

        auto delete_comment(UUID::uuid comment_id, bool recursive)
            -> ext::task<bool>;

        auto delete_post(UUID::uuid post_id) -> ext::task<>;

        auto delete_post_objects(
            UUID::uuid post_id,
            std::vector<UUID::uuid> objects
        ) -> ext::task<time_point>;

        auto delete_post_tag(UUID::uuid post_id, UUID::uuid tag_id)
            -> ext::task<>;

        auto delete_related_post(UUID::uuid post_id, UUID::uuid related)
            -> ext::task<>;

        auto delete_tag(UUID::uuid tag_id) -> ext::task<>;

        auto delete_tag_alias(UUID::uuid tag_id, std::string alias)
            -> ext::task<tag_name>;

        auto delete_tag_source(UUID::uuid tag_id, std::int64_t source_id)
            -> ext::task<>;

        auto get_comment(UUID::uuid comment_id)
            -> ext::task<std::optional<comment>>;

        auto get_comments(UUID::uuid post_id) -> ext::task<comment_tree>;

        auto get_object(UUID::uuid object_id)
            -> ext::task<std::optional<object>>;

        auto get_post(UUID::uuid post_id) -> ext::task<std::optional<post>>;

        auto get_posts(post_query query)
            -> ext::task<search_result<post_preview>>;

        auto get_server_info() -> ext::task<server_info>;

        auto get_tag(UUID::uuid tag_id) -> ext::task<std::optional<tag>>;

        auto get_tags(tag_query query) -> ext::task<search_result<tag_preview>>;

        auto set_comment_content(UUID::uuid comment_id, std::string content)
            -> ext::task<std::string>;

        auto set_post_description(UUID::uuid post_id, std::string description)
            -> ext::task<modification<std::optional<std::string>>>;

        auto set_post_title(UUID::uuid post_id, std::string title)
            -> ext::task<modification<std::optional<std::string>>>;

        auto set_tag_description(UUID::uuid tag_id, std::string description)
            -> ext::task<std::optional<std::string>>;

        auto set_tag_name(UUID::uuid tag_id, std::string new_name)
            -> ext::task<tag_name>;

        auto set_timer(seconds duration) -> ext::task<>;
    };

    static_assert(zipline::router_context<router_context>);
}
