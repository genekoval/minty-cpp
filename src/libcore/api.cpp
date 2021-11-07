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

    auto api::add_objects_url(
        std::string_view url
    ) -> std::vector<std::string> {
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
        const auto result = db->create_post(
            parts.title ?
                ext::trim(parts.title.value()) :
                parts.title,
            parts.description ?
                ext::trim(parts.description.value()) :
                parts.description,
            parts.objects,
            parts.tags
        );

        try {
            search->add_post(result);
        }
        catch (const std::runtime_error& ex) {
            ERROR()
                << "Failed to index post ("
                << result.id
                << "): "
                << ex.what();
        }

        return result.id;
    }

    auto api::add_post_objects(
        std::string_view post_id,
        const std::vector<std::string>& objects,
        unsigned int position
    ) -> std::vector<object_preview> {
        auto [new_objects, date_modified]= db->create_post_objects(
            post_id,
            objects,
            position
        );

        auto result = std::vector<object_preview>();
        for (auto&& obj : new_objects) {
            auto meta = bucket->meta(obj.id);
            result.emplace_back(
                std::move(obj),
                std::move(meta)
            );
        }

        search->update_post_date_modified(post_id, date_modified);
        return result;
    }

    auto api::add_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        db->create_post_tag(post_id, tag_id);
        search->add_post_tag(post_id, tag_id);
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
        const auto name_formatted = ext::trim(std::string(name));
        const auto id = db->create_tag(name_formatted);

        search->add_tag_alias(id, name_formatted);

        return id;
    }

    auto api::add_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        const auto alias_formatted = ext::trim(std::string(alias));
        const auto name = db->create_tag_alias(tag_id, alias_formatted);

        search->add_tag_alias(tag_id, alias_formatted);

        return name;
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
        search->delete_post(id);
    }

    auto api::delete_post_objects(
        std::string_view post_id,
        std::span<range> ranges
    ) -> void {
        search->update_post_date_modified(
            post_id,
            db->delete_post_objects(post_id, ranges)
        );
    }

    auto api::delete_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        db->delete_post_tag(post_id, tag_id);
        search->remove_post_tag(post_id, tag_id);
    }

    auto api::delete_tag(std::string_view id) -> void {
        db->delete_tag(id);
        search->delete_tag(id);
    }

    auto api::delete_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        const auto name = db->delete_tag_alias(tag_id, alias);
        search->delete_tag_alias(tag_id, alias);
        return name;
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

    auto api::get_posts(
        const post_query& query
    ) -> search_result<post_preview> {
        const auto result = search->find_posts(query);
        return {
            .total = result.total,
            .hits = db->read_posts(result.hits)
        };
    }

    auto api::get_tag(std::string_view id) -> tag {
        return tag(db->read_tag(id), db->read_tag_sources(id));
    }

    auto api::get_tags(const tag_query& query) -> search_result<tag_preview> {
        const auto result = search->find_tags(query);
        return {
            .total = result.total,
            .hits = db->read_tag_previews(result.hits)
        };
    }

    auto api::move_post_object(
        std::string_view post_id,
        unsigned int old_index,
        unsigned int new_index
    ) -> void {
        search->update_post_date_modified(
            post_id,
            db->move_post_object(post_id, old_index, new_index)
        );
    }

    auto api::prune() -> void {
        db->prune();

        auto result = fstore::remove_result();
        db->prune_objects([this, &result](auto objects) -> bool {
            try {
                result = bucket->remove(objects);
                return true;
            }
            catch (const std::runtime_error& ex) {
                ERROR() << "Object pruning has failed: " << ex.what();
            }

            return false;
        });

        if (result.objects_removed == 0) {
            INFO() << "No objects to prune";
            return;
        }

        INFO()
            << "Removed "
            << result.objects_removed << " object"
            << (result.objects_removed == 1 ? "" : "s")
            << " freeing " << data_size(result.space_freed).formatted;
    }

    auto api::reindex() -> void {
        search->delete_indices();
        search->create_indices();

        search->add_posts(db->read_post_search());
        search->add_tags(db->read_tag_text());
    }

    auto api::set_comment_content(
        std::string_view comment_id,
        std::string_view content
    ) -> std::string {
        const auto formatted = ext::replace(
            ext::trim(std::string(content)),
            std::regex("\r"),
            [](const auto&) -> std::string { return "\n"; }
        );

        if (formatted.empty()) {
            throw std::runtime_error("comment cannot be empty");
        }

        db->update_comment(comment_id, formatted);
        return formatted;
    }

    auto api::set_post_description(
        std::string_view post_id,
        std::string_view description
    ) -> modification<std::optional<std::string>> {
        const auto update = db->update_post_description(
            post_id,
            ext::replace(
                ext::trim(std::string(description)),
                std::regex("\r"),
                [](const auto&) -> std::string { return "\n"; }
            )
        );
        search->update_post_description(update);
        return {
            .date_modified = update.date_modified,
            .new_value = update.new_data
        };
    }

    auto api::set_post_title(
        std::string_view post_id,
        std::string_view title
    ) -> modification<std::optional<std::string>> {
        const auto update = db->update_post_title(
            post_id,
            ext::trim(std::string(title))
        );
        search->update_post_title(update);
        return {
            .date_modified = update.date_modified,
            .new_value = update.new_data
        };
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
        const auto name_formatted = ext::trim(std::string(new_name));
        const auto update  = db->update_tag_name(tag_id, name_formatted);

        if (update.old_name) {
            search->update_tag_name(
                tag_id,
                update.old_name.value(),
                name_formatted
            );
        }

        return update.names;
    }
}
