#include <minty/detail/http/repo.hpp>
#include <minty/model/comment.hpp>
#include <minty/model/modification.hpp>
#include <minty/model/object.hpp>
#include <minty/model/object_preview.hpp>
#include <minty/model/post.hpp>
#include <minty/model/search_result.hpp>
#include <minty/model/server_info.hpp>
#include <minty/model/source.hpp>
#include <minty/model/tag.hpp>
#include <minty/model/tag_name.hpp>
#include <minty/model/tag_preview.hpp>

namespace minty::detail::http {
    repo::repo(std::string_view base_url) : client(base_url) {}

    repo::repo(std::string_view base_url, ::http::session& session) :
        client(base_url, session) {}

    auto repo::add_comment(const UUID::uuid& post_id, std::string_view content)
        const -> request {
        auto request = client.post("comments", post_id);

        request.text_view(content);

        return request;
    }

    auto repo::add_object_data(const std::filesystem::path& path) const
        -> request {
        auto request = client.post("object");

        request.upload(path);

        return request;
    }

    auto repo::add_objects_url(std::string_view url) const -> request {
        auto request = client.post("object", "url");

        request.text_view(url);

        return request;
    }

    auto repo::add_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) const -> request {
        auto request = client.post("post", post_id, "objects");

        request.text(ext::join(objects, "\n"));

        return request;
    }

    auto repo::add_post_tag(const UUID::uuid& post_id, const UUID::uuid& tag_id)
        const -> request {
        return client.put("post", post_id, "tag", tag_id);
    }

    auto repo::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) const -> request {
        return client.put("post", post_id, "related", related);
    }

    auto repo::add_tag(std::string_view name) const -> request {
        return client.post("tag", name);
    }

    auto repo::add_tag_alias(const UUID::uuid& tag_id, std::string_view alias)
        const -> request {
        return client.put("tag", tag_id, "name", alias);
    }

    auto repo::add_tag_source(const UUID::uuid& tag_id, std::string_view url)
        const -> request {
        auto request = client.post("tag", tag_id, "source");

        request.text_view(url);

        return request;
    }

    auto repo::create_post(const UUID::uuid& draft) const -> request {
        return client.put("post", draft);
    }

    auto repo::create_post_draft() const -> request {
        return client.post("post");
    }

    auto repo::delete_comment(const UUID::uuid& comment_id, bool recursive)
        const -> request {
        auto request = client.del("comment", comment_id);

        if (recursive) request.query("r", recursive);

        return request;
    }

    auto repo::delete_post(const UUID::uuid& id) const -> request {
        return client.del("post", id);
    }

    auto repo::delete_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) const -> request {
        auto request = client.del("post", post_id, "objects");

        request.text(ext::join(objects, "\n"));

        return request;
    }

    auto repo::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) const -> request {
        return client.del("post", post_id, "tag", tag_id);
    }

    auto repo::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) const -> request {
        return client.del("post", post_id, "related", related);
    }

    auto repo::delete_tag(const UUID::uuid& id) const -> request {
        return client.del("tag", id);
    }

    auto repo::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) const -> request {
        return client.del("tag", tag_id, "name", alias);
    }

    auto repo::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) const -> request {
        return client.del("tag", tag_id, "source", source_id);
    }

    auto repo::get_comment(const UUID::uuid& comment_id) const -> request {
        return client.get("comment", comment_id);
    }

    auto repo::get_comments(const UUID::uuid& post_id) const -> request {
        return client.get("comments", post_id);
    }

    auto repo::get_object(const UUID::uuid& object_id) const -> request {
        return client.get("object", object_id);
    }

    auto repo::get_post(const UUID::uuid& id) const -> request {
        return client.get("post", id);
    }

    auto repo::get_posts(const post_query& query) const -> request {
        auto request = client.get("posts");

        request.query("size", query.size);

        if (query.from > 0) request.query("from", query.from);
        if (query.text) request.query("q", *query.text);
        if (!query.tags.empty()) {
            request.query("tags", ext::join(query.tags, ","));
        }
        if (query.visibility != visibility::pub) {
            request.query("vis", to_string(query.visibility));
        }

        if (query.sort != post_sort::created()) {
            request.query("sort", query.sort.value);
            request.query("order", query.sort.order);
        }

        return request;
    }

    auto repo::get_server_info() const -> request { return client.get(); }

    auto repo::get_tag(const UUID::uuid& id) const -> request {
        return client.get("tag", id);
    }

    auto repo::get_tags(const tag_query& query) const -> request {
        auto request = client.get("tags");

        request.query("name", query.name).query("size", query.size);

        if (query.from > 0) request.query("from", query.from);
        if (!query.exclude.empty()) {
            request.query("exclude", ext::join(query.exclude, ","));
        }

        return request;
    }

    auto repo::insert_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
        const UUID::uuid& destination
    ) const -> request {
        auto request = client.post("post", post_id, "objects", destination);

        request.text(ext::join(objects, "\n"));

        return request;
    }

    auto repo::reply(const UUID::uuid& parent_id, std::string_view content)
        const -> request {
        auto request = client.post("comment", parent_id);

        request.text_view(content);

        return request;
    }

    auto repo::set_comment_content(
        const UUID::uuid& comment_id,
        std::string_view content
    ) const -> request {
        auto request = client.put("comment", comment_id);

        request.text_view(content);

        return request;
    }

    auto repo::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) const -> request {
        auto request = client.put("post", post_id, "description");

        request.text_view(description);

        return request;
    }

    auto repo::set_post_title(const UUID::uuid& post_id, std::string_view title)
        const -> request {
        auto request = client.put("post", post_id, "title");

        request.text_view(title);

        return request;
    }

    auto repo::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) const -> request {
        auto request = client.put("tag", tag_id, "description");

        request.text_view(description);

        return request;
    }

    auto repo::set_tag_name(const UUID::uuid& tag_id, std::string_view new_name)
        const -> request {
        auto request = client.put("tag", tag_id, "name", new_name);

        request.query("main", true);

        return request;
    }
}
