#include <minty/async/async.hpp>
#include <minty/async/http/repo.hpp>

namespace minty::async::http {
    repo::repo(std::string_view base_url, ::http::session& session) :
        client(base_url, session) {}

    auto repo::add_comment(const UUID::uuid& post_id, std::string_view content)
        -> ext::task<comment_data> {
        co_return co_await client.add_comment(post_id, content)
            .send_async<comment_data>();
    }

    auto repo::add_object_data(const std::filesystem::path& path)
        -> ext::task<object_preview> {
        co_return co_await client.add_object_data(path)
            .send_async<object_preview>();
    }

    auto repo::add_objects(std::span<const std::string_view> arguments)
        -> ext::task<std::vector<object_preview>> {
        return detail::add_objects(arguments, *this);
    }

    auto repo::add_objects_url(std::string_view url)
        -> ext::task<std::vector<object_preview>> {
        co_return co_await client.add_objects_url(url)
            .send_async<std::vector<object_preview>>();
    }

    auto repo::add_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> ext::task<time_point> {
        co_return co_await client.add_post_objects(post_id, objects)
            .send_async<time_point>();
    }

    auto repo::add_post_tag(const UUID::uuid& post_id, const UUID::uuid& tag_id)
        -> ext::task<> {
        co_await client.add_post_tag(post_id, tag_id).send_async<>();
    }

    auto repo::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client.add_related_post(post_id, related).send_async<>();
    }

    auto repo::add_tag(std::string_view name) -> ext::task<UUID::uuid> {
        co_return co_await client.add_tag(name).send_async<UUID::uuid>();
    }

    auto repo::add_tag_alias(const UUID::uuid& tag_id, std::string_view alias)
        -> ext::task<tag_name> {
        co_return co_await client.add_tag_alias(tag_id, alias)
            .send_async<tag_name>();
    }

    auto repo::add_tag_source(const UUID::uuid& tag_id, std::string_view url)
        -> ext::task<source> {
        co_return co_await client.add_tag_source(tag_id, url)
            .send_async<source>();
    }

    auto repo::create_post(const UUID::uuid& draft) -> ext::task<> {
        co_await client.create_post(draft).send_async<>();
    }

    auto repo::create_post_draft() -> ext::task<UUID::uuid> {
        co_return co_await client.create_post_draft().send_async<UUID::uuid>();
    }

    auto repo::delete_comment(const UUID::uuid& comment_id, bool recursive)
        -> ext::task<> {
        co_await client.delete_comment(comment_id, recursive).send_async<>();
    }

    auto repo::delete_post(const UUID::uuid& id) -> ext::task<> {
        co_await client.delete_post(id).send_async<>();
    }

    auto repo::delete_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> ext::task<time_point> {
        co_return co_await client.delete_post_objects(post_id, objects)
            .send_async<time_point>();
    }

    auto repo::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client.delete_post_tag(post_id, tag_id).send_async<>();
    }

    auto repo::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client.delete_related_post(post_id, related).send_async<>();
    }

    auto repo::delete_tag(const UUID::uuid& id) -> ext::task<> {
        co_await client.delete_tag(id).send_async<>();
    }

    auto repo::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        co_return co_await client.delete_tag_alias(tag_id, alias)
            .send_async<tag_name>();
    }

    auto repo::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        co_await client.delete_tag_source(tag_id, source_id).send_async<>();
    }

    auto repo::get_comment(const UUID::uuid& comment_id) -> ext::task<comment> {
        co_return co_await client.get_comment(comment_id).send_async<comment>();
    }

    auto repo::get_comments(const UUID::uuid& post_id)
        -> ext::task<std::vector<comment_data>> {
        co_return co_await client.get_comments(post_id)
            .send_async<std::vector<comment_data>>();
    }

    auto repo::get_object(const UUID::uuid& object_id) -> ext::task<object> {
        co_return co_await client.get_object(object_id).send_async<object>();
    }

    auto repo::get_post(const UUID::uuid& id) -> ext::task<post> {
        co_return co_await client.get_post(id).send_async<post>();
    }

    auto repo::get_posts(const post_query& query)
        -> ext::task<search_result<post_preview>> {
        co_return co_await client.get_posts(query)
            .send_async<search_result<post_preview>>();
    }

    auto repo::get_server_info() -> ext::task<server_info> {
        co_return co_await client.get_server_info().send_async<server_info>();
    }

    auto repo::get_tag(const UUID::uuid& id) -> ext::task<tag> {
        co_return co_await client.get_tag(id).send_async<tag>();
    }

    auto repo::get_tags(const tag_query& query)
        -> ext::task<search_result<tag_preview>> {
        co_return co_await client.get_tags(query)
            .send_async<search_result<tag_preview>>();
    }

    auto repo::insert_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
        const UUID::uuid& destination
    ) -> ext::task<time_point> {
        co_return co_await client
            .insert_post_objects(post_id, objects, destination)
            .send_async<time_point>();
    }

    auto repo::reply(const UUID::uuid& parent_id, std::string_view content)
        -> ext::task<comment_data> {
        co_return co_await client.reply(parent_id, content)
            .send_async<comment_data>();
    }

    auto repo::set_comment_content(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> ext::task<std::string> {
        co_return co_await client.set_comment_content(comment_id, content)
            .send_async<std::string>();
    }

    auto repo::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> ext::task<modification<std::string>> {
        co_return co_await client.set_post_description(post_id, description)
            .send_async<modification<std::string>>();
    }

    auto repo::set_post_title(const UUID::uuid& post_id, std::string_view title)
        -> ext::task<modification<std::string>> {
        co_return co_await client.set_post_title(post_id, title)
            .send_async<modification<std::string>>();
    }

    auto repo::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> ext::task<std::string> {
        co_return co_await client.set_tag_description(tag_id, description)
            .send_async<std::string>();
    }

    auto repo::set_tag_name(const UUID::uuid& tag_id, std::string_view new_name)
        -> ext::task<tag_name> {
        co_return co_await client.set_tag_name(tag_id, new_name)
            .send_async<tag_name>();
    }
}
