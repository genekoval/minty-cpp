#include <minty/sync/http/repo.hpp>
#include <minty/sync/sync.hpp>

#include <ext/string.h>
#include <uri/uri>

namespace minty::sync::http {
    repo::repo(std::string_view base_url) : client(base_url) {}

    repo::repo(std::string_view base_url, std::string_view objects_url) :
        client(base_url),
        bucket_storage(std::in_place_type<std::string>, objects_url) {}

    auto repo::add_comment(const UUID::uuid& post_id, std::string_view content)
        -> comment_data {
        return client.add_comment(post_id, content).send<comment_data>();
    }

    auto repo::add_object_data(const std::filesystem::path& path)
        -> object_preview {
        return client.add_object_data(path).send<object_preview>();
    }

    auto repo::add_objects(std::span<const std::string_view> arguments)
        -> std::vector<object_preview> {
        return detail::add_objects(arguments, *this);
    }

    auto repo::add_objects_url(std::string_view url)
        -> std::vector<object_preview> {
        return client.add_objects_url(url).send<std::vector<object_preview>>();
    }

    auto repo::add_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> time_point {
        return client.add_post_objects(post_id, objects).send<time_point>();
    }

    auto repo::add_post_tag(const UUID::uuid& post_id, const UUID::uuid& tag_id)
        -> void {
        client.add_post_tag(post_id, tag_id).send<>();
    }

    auto repo::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        client.add_related_post(post_id, related).send<>();
    }

    auto repo::add_tag(std::string_view name) -> UUID::uuid {
        return client.add_tag(name).send<UUID::uuid>();
    }

    auto repo::add_tag_alias(const UUID::uuid& tag_id, std::string_view alias)
        -> tag_name {
        return client.add_tag_alias(tag_id, alias).send<tag_name>();
    }

    auto repo::add_tag_source(const UUID::uuid& tag_id, std::string_view url)
        -> source {
        return client.add_tag_source(tag_id, url).send<source>();
    }

    auto repo::bucket() -> fstore::sync::http::bucket& {
        if (auto* const bucket =
                std::get_if<fstore::sync::http::bucket>(&bucket_storage))
            return *bucket;

        const auto info = get_server_info();
        const auto* location = &info.object_source.location;

        if (const auto* const string =
                std::get_if<std::string>(&bucket_storage))
            location = string;

        return bucket_storage.emplace<fstore::sync::http::bucket>(
            *location,
            info.object_source.bucket
        );
    }

    auto repo::create_post(const UUID::uuid& draft) -> void {
        client.create_post(draft).send<>();
    }

    auto repo::create_post_draft() -> UUID::uuid {
        return client.create_post_draft().send<UUID::uuid>();
    }

    auto repo::delete_comment(const UUID::uuid& comment_id, bool recursive)
        -> void {
        client.delete_comment(comment_id, recursive).send<>();
    }

    auto repo::delete_post(const UUID::uuid& id) -> void {
        client.delete_post(id).send<>();
    }

    auto repo::delete_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> time_point {
        return client.delete_post_objects(post_id, objects).send<time_point>();
    }

    auto repo::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> void {
        client.delete_post_tag(post_id, tag_id).send();
    }

    auto repo::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> void {
        client.delete_related_post(post_id, related).send();
    }

    auto repo::delete_tag(const UUID::uuid& id) -> void {
        client.delete_tag(id).send();
    }

    auto repo::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> tag_name {
        return client.delete_tag_alias(tag_id, alias).send<tag_name>();
    }

    auto repo::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> void {
        client.delete_tag_source(tag_id, source_id).send();
    }

    auto repo::download_object(
        const UUID::uuid& object_id,
        const std::filesystem::path& location
    ) -> void {
        bucket().download_object(object_id, location);
    }

    auto repo::get_comment(const UUID::uuid& comment_id) -> comment {
        return client.get_comment(comment_id).send<comment>();
    }

    auto repo::get_comments(const UUID::uuid& post_id)
        -> std::vector<comment_data> {
        return client.get_comments(post_id).send<std::vector<comment_data>>();
    }

    auto repo::get_object(const UUID::uuid& object_id) -> object {
        return client.get_object(object_id).send<object>();
    }

    auto repo::get_object_data(const UUID::uuid& object_id) -> std::string {
        return bucket().get_object(object_id);
    }

    auto repo::get_object_data(const UUID::uuid& object_id, FILE* file)
        -> void {
        bucket().get_object(object_id, file);
    }

    auto repo::get_post(const UUID::uuid& id) -> post {
        return client.get_post(id).send<post>();
    }

    auto repo::get_posts(const post_query& query)
        -> search_result<post_preview> {
        return client.get_posts(query).send<search_result<post_preview>>();
    }

    auto repo::get_server_info() -> server_info {
        return client.get_server_info().send<server_info>();
    }

    auto repo::get_tag(const UUID::uuid& id) -> tag {
        return client.get_tag(id).send<tag>();
    }

    auto repo::get_tags(const tag_query& query) -> search_result<tag_preview> {
        return client.get_tags(query).send<search_result<tag_preview>>();
    }

    auto repo::insert_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
        const UUID::uuid& destination
    ) -> time_point {
        return client.insert_post_objects(post_id, objects, destination)
            .send<time_point>();
    }

    auto repo::reply(const UUID::uuid& parent_id, std::string_view content)
        -> comment_data {
        return client.reply(parent_id, content).send<comment_data>();
    }

    auto repo::set_comment_content(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> std::string {
        return client.set_comment_content(comment_id, content)
            .send<std::string>();
    }

    auto repo::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> modification<std::string> {
        return client.set_post_description(post_id, description)
            .send<modification<std::string>>();
    }

    auto repo::set_post_title(const UUID::uuid& post_id, std::string_view title)
        -> modification<std::string> {
        return client.set_post_title(post_id, title)
            .send<modification<std::string>>();
    }

    auto repo::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> std::string {
        return client.set_tag_description(tag_id, description)
            .send<std::string>();
    }

    auto repo::set_tag_name(const UUID::uuid& tag_id, std::string_view new_name)
        -> tag_name {
        return client.set_tag_name(tag_id, new_name).send<tag_name>();
    }
}
