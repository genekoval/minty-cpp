#include <internal/server/router.hpp>

namespace minty::server {
    router_context::router_context(core::repo& repo, const server_info& info) :
        repo(&repo),
        info(&info) {}

    auto router_context::add_comment(UUID::uuid post_id, std::string content)
        -> ext::task<comment_data> {
        co_return co_await repo->add_comment(post_id, content);
    }

    auto router_context::add_object_data(stream stream)
        -> ext::task<object_preview> {
        const auto size = co_await stream.size();

        co_return co_await repo->add_object_data(
            size,
            [&stream](fstore::part& part) -> ext::task<> {
                co_await stream.read(
                    [&part](std::span<const std::byte> chunk) -> ext::task<> {
                        co_await part.write(chunk);
                    }
                );
            }
        );
    }

    auto router_context::add_objects_url(std::string url)
        -> ext::task<std::vector<object_preview>> {
        co_return co_await repo->add_objects_url(url);
    }

    auto router_context::add_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects,
        std::optional<UUID::uuid> destination
    ) -> ext::task<time_point> {
        co_return co_await repo
            ->add_post_objects(post_id, objects, destination);
    }

    auto router_context::add_post_tag(UUID::uuid post_id, UUID::uuid tag_id)
        -> ext::task<> {
        co_await repo->add_post_tag(post_id, tag_id);
    }

    auto router_context::add_related_post(
        UUID::uuid post_id,
        UUID::uuid related
    ) -> ext::task<> {
        co_await repo->add_related_post(post_id, related);
    }

    auto router_context::add_reply(UUID::uuid parent_id, std::string content)
        -> ext::task<comment_data> {
        co_return co_await repo->add_reply(parent_id, content);
    }

    auto router_context::add_tag(std::string name) -> ext::task<UUID::uuid> {
        co_return co_await repo->add_tag(name);
    }

    auto router_context::add_tag_alias(UUID::uuid tag_id, std::string alias)
        -> ext::task<tag_name> {
        co_return co_await repo->add_tag_alias(tag_id, alias);
    }

    auto router_context::add_tag_source(UUID::uuid tag_id, std::string url)
        -> ext::task<source> {
        co_return co_await repo->add_tag_source(tag_id, url);
    }

    auto router_context::create_post(UUID::uuid post_id) -> ext::task<> {
        co_await repo->create_post(post_id);
    }

    auto router_context::create_post_draft() -> ext::task<UUID::uuid> {
        co_return co_await repo->create_post_draft();
    }

    auto router_context::delete_comment_tree(UUID::uuid comment_id)
        -> ext::task<bool> {
        co_return co_await repo->delete_comment_tree(comment_id);
    }

    auto router_context::delete_post(UUID::uuid post_id) -> ext::task<> {
        co_await repo->delete_post(post_id);
    }

    auto router_context::delete_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects
    ) -> ext::task<time_point> {
        co_return co_await repo->delete_post_objects(post_id, objects);
    }

    auto router_context::delete_post_tag(UUID::uuid post_id, UUID::uuid tag_id)
        -> ext::task<> {
        co_await repo->delete_post_tag(post_id, tag_id);
    }

    auto router_context::delete_related_post(
        UUID::uuid post_id,
        UUID::uuid related
    ) -> ext::task<> {
        co_await repo->delete_related_post(post_id, related);
    }

    auto router_context::delete_tag(UUID::uuid tag_id) -> ext::task<> {
        co_await repo->delete_tag(tag_id);
    }

    auto router_context::delete_tag_alias(UUID::uuid tag_id, std::string alias)
        -> ext::task<tag_name> {
        co_return co_await repo->delete_tag_alias(tag_id, alias);
    }

    auto router_context::delete_tag_source(
        UUID::uuid tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        co_await repo->delete_tag_source(tag_id, source_id);
    }

    auto router_context::get_comment(UUID::uuid comment_id)
        -> ext::task<std::optional<comment>> {
        co_return co_await repo->get_comment(comment_id);
    }

    auto router_context::get_comments(UUID::uuid post_id)
        -> ext::task<comment_tree> {
        co_return co_await repo->get_comments(post_id);
    }

    auto router_context::get_object(UUID::uuid object_id)
        -> ext::task<std::optional<object>> {
        co_return co_await repo->get_object(object_id);
    }

    auto router_context::get_post(UUID::uuid post_id)
        -> ext::task<std::optional<post>> {
        co_return co_await repo->get_post(post_id);
    }

    auto router_context::get_posts(post_query query)
        -> ext::task<search_result<post_preview>> {
        co_return co_await repo->get_posts(query);
    }

    auto router_context::get_server_info() -> ext::task<server_info> {
        co_return *info;
    }

    auto router_context::get_tag(UUID::uuid tag_id)
        -> ext::task<std::optional<tag>> {
        co_return co_await repo->get_tag(tag_id);
    }

    auto router_context::get_tags(tag_query query)
        -> ext::task<search_result<tag_preview>> {
        co_return co_await repo->get_tags(query);
    }

    auto router_context::set_comment_content(
        UUID::uuid comment_id,
        std::string content
    ) -> ext::task<std::string> {
        co_return co_await repo->set_comment_content(comment_id, content);
    }

    auto router_context::set_post_description(
        UUID::uuid post_id,
        std::string description
    ) -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await repo->set_post_description(post_id, description);
    }

    auto router_context::set_post_title(UUID::uuid post_id, std::string title)
        -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await repo->set_post_title(post_id, title);
    }

    auto router_context::set_tag_description(
        UUID::uuid tag_id,
        std::string description
    ) -> ext::task<std::optional<std::string>> {
        co_return co_await repo->set_tag_description(tag_id, description);
    }

    auto router_context::set_tag_name(UUID::uuid tag_id, std::string new_name)
        -> ext::task<tag_name> {
        co_return co_await repo->set_tag_name(tag_id, new_name);
    }

    auto router_context::set_timer(seconds duration) -> ext::task<> {
        co_await netcore::sleep_for(duration);
    }
}
