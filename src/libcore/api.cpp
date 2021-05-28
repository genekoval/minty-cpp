#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/preview.h>
#include <minty/core/search.h>

#include <ext/string.h>
#include <fmt/format.h>
#include <uri/uri>

namespace minty::core {
    api::api(
        repo::db::database& db,
        fstore::bucket& bucket,
        downloader& dl,
        search_engine& search
    ) :
        db(&db),
        bucket(&bucket),
        dl(&dl),
        previews(bucket),
        search(&search)
    {}

    auto api::add_comment(
        std::string_view post_id,
        std::optional<std::string_view> parent_id,
        std::string_view content
    ) -> comment {
        const auto comment = db->create_comment(post_id, parent_id, content);
        return {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto api::add_object_data(
        std::size_t stream_size,
        std::function<void(fstore::part&&)> pipe
    ) -> std::string {
        const auto object = bucket->add({}, stream_size, pipe);
        db->create_object(object.id, previews.generate_preview(object), {});
        return object.id;
    }

    auto api::add_object_local(std::string_view path) -> std::string {
        const auto object = bucket->add(path);
        db->create_object(object.id, previews.generate_preview(object), {});
        return object.id;
    }

    auto api::add_object_url(std::string_view url) -> std::vector<std::string> {
        auto objects = std::vector<fstore::object_meta>();

        const auto used_scraper = dl->fetch(url, [&](auto& file) {
            objects.push_back(bucket->add(
                {},
                file.size(),
                [&file](auto&& part) {
                    file.read([&part](auto&& chunk) {
                        part.write(chunk);
                    });
                }
            ));
        });

        const auto src = used_scraper ?
            add_source(url).id : std::optional<std::string>();

        auto result = std::vector<std::string>();

        for (const auto& obj : objects) {
            db->create_object(obj.id, previews.generate_preview(obj), src);
            result.push_back(obj.id);
        }

        return result;
    }

    auto api::add_post(post_parts parts) -> std::string {
        return db->create_post(
            ext::trim(parts.title),
            ext::trim(parts.description),
            parts.objects,
            parts.tags
        );
    }

    auto api::add_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        db->create_post_tag(post_id, tag_id);
    }

    auto api::add_source(std::string_view url) -> source {
        constexpr auto host_prefix = std::string_view("www.");

        const auto url_string = ext::trim(std::string(url));
        const auto uri = uri::uri(url_string);

        const auto scheme = uri.scheme();
        auto host = uri.host();
        if (host.starts_with(host_prefix)) {
            host = host.substr(host_prefix.size());
        }

        const auto site_opt = db->read_site(scheme, host);
        const auto site_id = site_opt ? *site_opt : add_site(scheme, host);

        auto resource = uri.pathname();

        if (!uri.query().empty()) resource.append(
            fmt::format("?{}", uri.query())
        );
        if (!uri.fragment().empty()) resource.append(
            fmt::format("#{}", uri.fragment())
        );

        return db->create_source(site_id, resource);
    }

    auto api::add_site(
        std::string_view scheme,
        std::string_view host
    ) -> std::string {
        auto icon_id = std::optional<std::string>();

        dl->get_site_icon(
            fmt::format("{}://{}", scheme, host),
            [&](auto& stream) {
                const auto object = bucket->add(
                    {},
                    stream.size(),
                    [&stream] (auto&& part) {
                        stream.read([&part](auto&& chunk) {
                            part.write(chunk);
                        });
                    }
                );

                icon_id = object.id;
            }
        );

        return db->create_site(scheme, host, icon_id).id;
    }

    auto api::add_tag(std::string_view name) -> std::string {
        return db->create_tag(ext::trim(std::string(name)));
    }

    auto api::add_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        return db->create_tag_alias(tag_id, ext::trim(std::string(alias)));
    }

    auto api::add_tag_source(
        std::string_view tag_id,
        std::string_view url
    ) -> source {
        const auto src = add_source(url);
        db->create_tag_source(tag_id, src.id);
        return src;
    }

    auto api::delete_post(std::string_view id) -> void {
        db->delete_post(id);
    }

    auto api::delete_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        db->delete_post_tag(post_id, tag_id);
    }

    auto api::delete_tag(std::string_view id) -> void {
        db->delete_tag(id);
    }

    auto api::delete_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        return db->delete_tag_alias(tag_id, alias);
    }

    auto api::delete_tag_source(
        std::string_view tag_id,
        std::string_view source_id
    ) -> void {
        db->delete_tag_source(tag_id, source_id);
    }

    auto api::get_comments(std::string_view post_id) -> comment_tree {
        const auto entities = db->read_comments(post_id);
        return build_tree(entities);
    }

    auto api::get_object(std::string_view object_id) -> object {
        return object(
            db->read_object(object_id),
            bucket->meta(object_id),
            db->read_object_posts(object_id)
        );
    }

    auto api::get_post(std::string_view id) -> post {
        auto data = db->read_post(id);
        auto objects = db->read_post_objects(id);

        auto result = post {
            .id = data.id,
            .title = data.title,
            .description = data.description,
            .date_created = data.date_created,
            .date_modified = data.date_modified,
            .tags = db->read_post_tags(id)
        };

        for (auto&& obj : objects) {
            auto meta = bucket->meta(obj.id);
            result.objects.emplace_back(
                std::move(obj),
                std::move(meta)
            );
        }

        return result;
    }

    auto api::get_tag(std::string_view id) -> tag {
        return tag(db->read_tag(id), db->read_tag_sources(id));
    }

    auto api::get_tags_by_name(
        std::string_view term
    ) -> std::vector<tag_preview> {
        const auto ids = search->find_tags_by_name(term);
        return db->read_tag_previews(ids);
    }

    auto api::get_tag_posts(
        std::string_view tag_id
    ) -> std::vector<post_preview> {
        return db->read_tag_posts(tag_id);
    }

    auto api::get_tag_previews() -> std::vector<tag_preview> {
        return db->read_tag_previews_all();
    }

    auto api::set_post_description(
        std::string_view post_id,
        std::string_view description
    ) -> std::optional<std::string> {
        return db->update_post_description(
            post_id,
            ext::replace(
                ext::trim(std::string(description)),
                std::regex("\r"),
                [](const auto&) -> std::string { return "\n"; }
            )
        );
    }

    auto api::set_post_title(
        std::string_view post_id,
        std::string_view title
    ) -> std::optional<std::string> {
        return db->update_post_title(post_id, ext::trim(std::string(title)));
    }

    auto api::set_tag_description(
        std::string_view tag_id,
        std::string_view description
    ) -> std::optional<std::string> {

        return db->update_tag_description(
            tag_id,
            ext::replace(
                ext::trim(std::string(description)),
                std::regex("\r"),
                [](const auto&) -> std::string { return "\n"; }
            )
        );
    }

    auto api::set_tag_name(
        std::string_view tag_id,
        std::string_view new_name
    ) -> tag_name {
        return db->update_tag_name(tag_id, ext::trim(std::string(new_name)));
    }
}
