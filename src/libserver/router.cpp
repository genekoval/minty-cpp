#include <internal/server/router.hpp>

namespace minty::server {
    router_context::router_context(core::api& api, const server_info& info) :
        api(&api),
        info(&info)
   {}

    auto router_context::add_comment(
        UUID::uuid post_id,
        std::string content
    ) -> ext::task<comment_data> {
        co_return co_await api->add_comment(post_id, content);
    }

    auto router_context::add_object_data(
        stream stream
    ) -> ext::task<object_preview> {
        const auto size = co_await stream.size();

        co_return co_await api->add_object_data(size, [&stream](
            fstore::part& part
        ) -> ext::task<> {
            co_await stream.read([&part](
                std::span<const std::byte> chunk
            ) -> ext::task<> {
                co_await part.write(chunk);
            });
        });
    }

    auto router_context::add_objects_url(
        std::string url
    ) -> ext::task<std::vector<object_preview>> {
        co_return co_await api->add_objects_url(url);
    }

    auto router_context::add_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects,
        std::int16_t position
    ) -> ext::task<time_point> {
        co_return co_await api->add_post_objects(post_id, objects, position);
    }

    auto router_context::add_post_tag(
        UUID::uuid post_id,
        UUID::uuid tag_id
    ) -> ext::task<> {
        co_await api->add_post_tag(post_id, tag_id);
    }

    auto router_context::add_related_post(
        UUID::uuid post_id,
        UUID::uuid related
    ) -> ext::task<> {
        co_await api->add_related_post(post_id, related);
    }

    auto router_context::add_reply(
        UUID::uuid parent_id,
        std::string content
    ) -> ext::task<comment_data> {
        co_return co_await api->add_reply(parent_id, content);
    }

    auto router_context::add_tag(std::string name) -> ext::task<UUID::uuid> {
        co_return co_await api->add_tag(name);
    }

    auto router_context::add_tag_alias(
        UUID::uuid tag_id,
        std::string alias
    ) -> ext::task<tag_name> {
        co_return co_await api->add_tag_alias(tag_id, alias);
    }

    auto router_context::add_tag_source(
        UUID::uuid tag_id,
        std::string url
    ) -> ext::task<source> {
        co_return co_await api->add_tag_source(tag_id, url);
    }

    auto router_context::create_post(UUID::uuid post_id) -> ext::task<> {
        co_await api->create_post(post_id);
    }

    auto router_context::create_post_draft() -> ext::task<UUID::uuid> {
        co_return co_await api->create_post_draft();
    }

    auto router_context::delete_post(UUID::uuid post_id) -> ext::task<> {
        co_await api->delete_post(post_id);
    }

    auto router_context::delete_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects
    ) -> ext::task<time_point> {
        co_return co_await api->delete_post_objects(post_id, objects);
    }

    auto router_context::delete_post_tag(
        UUID::uuid post_id,
        UUID::uuid tag_id
    ) -> ext::task<> {
        co_await api->delete_post_tag(post_id, tag_id);
    }

    auto router_context::delete_related_post(
        UUID::uuid post_id,
        UUID::uuid related
    ) -> ext::task<> {
        co_await api->delete_related_post(post_id, related);
    }

    auto router_context::delete_tag(UUID::uuid tag_id) -> ext::task<> {
        co_await api->delete_tag(tag_id);
    }

    auto router_context::delete_tag_alias(
        UUID::uuid tag_id,
        std::string alias
    ) -> ext::task<tag_name> {
        co_return co_await api->delete_tag_alias(tag_id, alias);
    }

    auto router_context::delete_tag_source(
        UUID::uuid tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        co_await api->delete_tag_source(tag_id, source_id);
    }

    auto router_context::get_comment(
        UUID::uuid comment_id
    ) -> ext::task<comment> {
        co_return co_await api->get_comment(comment_id);
    }

    auto router_context::get_comments(
        UUID::uuid post_id
    ) -> ext::task<comment_tree> {
        co_return co_await api->get_comments(post_id);
    }

    auto router_context::get_object(
        UUID::uuid object_id
    ) -> ext::task<object> {
        co_return co_await api->get_object(object_id);
    }

    auto router_context::get_post(UUID::uuid post_id) -> ext::task<post> {
        co_return co_await api->get_post(post_id);
    }

    auto router_context::get_posts(
        post_query query
    ) -> ext::task<search_result<post_preview>> {
        co_return co_await api->get_posts(query);
    }

    auto router_context::get_server_info() -> ext::task<server_info> {
        co_return *info;
    }

    auto router_context::get_tag(UUID::uuid tag_id) -> ext::task<tag> {
        co_return co_await api->get_tag(tag_id);
    }

    auto router_context::get_tags(
        tag_query query
    ) -> ext::task<search_result<tag_preview>> {
        co_return co_await api->get_tags(query);
    }

    auto router_context::move_post_objects(
        UUID::uuid post_id,
        std::vector<UUID::uuid> objects,
        std::optional<UUID::uuid> destination
    ) -> ext::task<time_point> {
        co_return co_await api->move_post_objects(
            post_id,
            objects,
            destination
        );
    }

    auto router_context::set_comment_content(
        UUID::uuid comment_id,
        std::string content
    ) -> ext::task<std::string> {
        co_return co_await api->set_comment_content(comment_id, content);
    }

    auto router_context::set_post_description(
        UUID::uuid post_id,
        std::string description
    ) -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await api->set_post_description(post_id, description);
    }

    auto router_context::set_post_title(
        UUID::uuid post_id,
        std::string title
    ) -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await api->set_post_title(post_id, title);
    }

    auto router_context::set_tag_description(
        UUID::uuid tag_id,
        std::string description
    ) -> ext::task<std::optional<std::string>> {
        co_return co_await api->set_tag_description(tag_id, description);
    }

    auto router_context::set_tag_name(
        UUID::uuid tag_id,
        std::string new_name
    ) -> ext::task<tag_name> {
        co_return co_await api->set_tag_name(tag_id, new_name);
    }
}
