#include <minty/async/async.hpp>
#include <minty/async/http/repo.hpp>

namespace minty::async::http {
    repo::repo(std::string_view base_url, ::http::session& session) :
        client(base_url, session) {}

    auto repo::add_comment(const UUID::uuid& post_id, std::string_view content)
        -> ext::task<comment_data> {
        co_return co_await client.add_comment(post_id, content)
            .json_task<comment_data>();
    }

    auto repo::add_object_data(const std::filesystem::path& path)
        -> ext::task<object_preview> {
        co_return co_await client.add_object_data(path)
            .json_task<object_preview>();
    }

    auto repo::add_objects(std::span<const std::string_view> arguments)
        -> ext::task<std::vector<object_preview>> {
        return detail::add_objects(arguments, *this);
    }

    auto repo::add_objects_url(std::string_view url)
        -> ext::task<std::vector<object_preview>> {
        co_return co_await client.add_objects_url(url)
            .json_task<std::vector<object_preview>>();
    }

    auto repo::add_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> ext::task<time_point> {
        co_return co_await client.add_post_objects(post_id, objects)
            .json_task<time_point>();
    }

    auto repo::add_post_tag(const UUID::uuid& post_id, const UUID::uuid& tag_id)
        -> ext::task<> {
        co_await client.add_post_tag(post_id, tag_id).send_task();
    }

    auto repo::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client.add_related_post(post_id, related).send_task();
    }

    auto repo::add_tag(std::string_view name) -> ext::task<UUID::uuid> {
        co_return co_await client.add_tag(name).json_task<UUID::uuid>();
    }

    auto repo::add_tag_alias(const UUID::uuid& tag_id, std::string_view alias)
        -> ext::task<tag_name> {
        co_return co_await client.add_tag_alias(tag_id, alias)
            .json_task<tag_name>();
    }

    auto repo::add_tag_source(const UUID::uuid& tag_id, std::string_view url)
        -> ext::task<source> {
        co_return co_await client.add_tag_source(tag_id, url)
            .json_task<source>();
    }

    auto repo::create_post(const UUID::uuid& draft) -> ext::task<> {
        co_await client.create_post(draft).send_task();
    }

    auto repo::create_post_draft() -> ext::task<UUID::uuid> {
        co_return co_await client.create_post_draft().json_task<UUID::uuid>();
    }

    auto repo::delete_comment_tree(const UUID::uuid& comment_id)
        -> ext::task<bool> {
        try {
            co_await client.delete_comment_tree(comment_id).send_task();
            co_return true;
        }
        catch (const ::http::error_code& ex) {
            if (ex.code() == 404) co_return false;
            throw;
        }
    }

    auto repo::delete_post(const UUID::uuid& id) -> ext::task<> {
        co_await client.delete_post(id).send_task();
    }

    auto repo::delete_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> ext::task<time_point> {
        co_return co_await client.delete_post_objects(post_id, objects)
            .json_task<time_point>();
    }

    auto repo::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client.delete_post_tag(post_id, tag_id).send_task();
    }

    auto repo::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client.delete_related_post(post_id, related).send_task();
    }

    auto repo::delete_tag(const UUID::uuid& id) -> ext::task<> {
        co_await client.delete_tag(id).send_task();
    }

    auto repo::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        co_return co_await client.delete_tag_alias(tag_id, alias)
            .json_task<tag_name>();
    }

    auto repo::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        co_await client.delete_tag_source(tag_id, source_id).send_task();
    }

    auto repo::get_comment(const UUID::uuid& comment_id)
        -> ext::task<std::optional<comment>> {
        co_return co_await client.get_comment(comment_id)
            .try_json_task<comment>();
    }

    auto repo::get_comments(const UUID::uuid& post_id)
        -> ext::task<std::vector<comment_data>> {
        co_return co_await client.get_comments(post_id)
            .json_task<std::vector<comment_data>>();
    }

    auto repo::get_object(const UUID::uuid& object_id)
        -> ext::task<std::optional<object>> {
        co_return co_await client.get_object(object_id).try_json_task<object>();
    }

    auto repo::get_post(const UUID::uuid& id)
        -> ext::task<std::optional<post>> {
        co_return co_await client.get_post(id).try_json_task<post>();
    }

    auto repo::get_posts(const post_query& query)
        -> ext::task<search_result<post_preview>> {
        co_return co_await client.get_posts(query)
            .json_task<search_result<post_preview>>();
    }

    auto repo::get_server_info() -> ext::task<server_info> {
        co_return co_await client.get_server_info().json_task<server_info>();
    }

    auto repo::get_tag(const UUID::uuid& id) -> ext::task<std::optional<tag>> {
        co_return co_await client.get_tag(id).try_json_task<tag>();
    }

    auto repo::get_tags(const tag_query& query)
        -> ext::task<search_result<tag_preview>> {
        co_return co_await client.get_tags(query)
            .json_task<search_result<tag_preview>>();
    }

    auto repo::insert_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
        const UUID::uuid& destination
    ) -> ext::task<time_point> {
        co_return co_await client
            .insert_post_objects(post_id, objects, destination)
            .json_task<time_point>();
    }

    auto repo::reply(const UUID::uuid& parent_id, std::string_view content)
        -> ext::task<comment_data> {
        co_return co_await client.reply(parent_id, content)
            .json_task<comment_data>();
    }

    auto repo::set_comment_content(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> ext::task<std::string> {
        co_return co_await client.set_comment_content(comment_id, content)
            .json_task<std::string>();
    }

    auto repo::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await client.set_post_description(post_id, description)
            .json_task<modification<std::optional<std::string>>>();
    }

    auto repo::set_post_title(const UUID::uuid& post_id, std::string_view title)
        -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await client.set_post_title(post_id, title)
            .json_task<modification<std::optional<std::string>>>();
    }

    auto repo::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> ext::task<std::optional<std::string>> {
        co_return co_await client.set_tag_description(tag_id, description)
            .json_task<std::optional<std::string>>();
    }

    auto repo::set_tag_name(const UUID::uuid& tag_id, std::string_view new_name)
        -> ext::task<tag_name> {
        co_return co_await client.set_tag_name(tag_id, new_name)
            .json_task<tag_name>();
    }
}
