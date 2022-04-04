#include "utility.h"

#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/preview.h>
#include <minty/core/search.h>

#include <uri/uri>

namespace minty::core {
    api::api(
        repo::db::database& db,
        object_store& objects,
        downloader& dl,
        search_engine& search
    ) :
        db(&db),
        objects(&objects),
        dl(&dl),
        previews(objects),
        search(&search)
    {}

    auto api::add_comment(
        std::string_view post_id,
        std::string_view content
    ) -> comment {
        TIMBER_FUNC();

        const auto comment = db->create_comment(
            post_id,
            format_comment(content)
        );

        return {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto api::add_object(
        fstore::object_meta&& object,
        const std::optional<std::string>& src
    ) -> object_preview {
        auto preview_id = previews.generate_preview(object);

        db->create_object(object.id, preview_id, src);

        return {
            std::move(object.id),
            std::move(preview_id),
            std::move(object.type),
            std::move(object.subtype)
        };
    }

    auto api::add_object_data(
        std::size_t stream_size,
        std::function<void(fstore::part&&)> pipe
    ) -> object_preview {
        TIMBER_FUNC();

        return add_object(objects->add({}, stream_size, pipe), {});
    }

    auto api::add_object_local(std::string_view path) -> core::object_preview {
        TIMBER_FUNC();

        return add_object(objects->add(path), {});
    }

    auto api::add_objects_url(
        std::string_view url
    ) -> std::vector<core::object_preview> {
        TIMBER_FUNC();

        auto objects = std::vector<fstore::object_meta>();

        const auto used_scraper = dl->fetch(url, [&](auto& file) {
            objects.push_back(this->objects->add(
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

        auto result = std::vector<object_preview>();

        for (auto&& obj : objects) {
            result.push_back(add_object(std::move(obj), src));
        }

        return result;
    }

    auto api::add_post(post_parts parts) -> std::string {
        TIMBER_FUNC();

        const auto result = db->create_post(
            format_title(parts.title),
            format_description(parts.description),
            parts.objects,
            parts.tags
        );

        try {
            search->add_post(result);
        }
        catch (const std::runtime_error& ex) {
            TIMBER_ERROR("Failed to index post ({}): {}", result.id, ex.what());
        }

        return result.id;
    }

    auto api::add_post_objects(
        std::string_view post_id,
        const std::vector<std::string>& objects,
        std::int16_t position
    ) -> std::string {
        TIMBER_FUNC();

        const auto date_modified =
            db->create_post_objects(post_id, objects, position);

        search->update_post_date_modified(post_id, date_modified);

        return date_modified;
    }

    auto api::add_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        TIMBER_FUNC();

        db->create_post_tag(post_id, tag_id);
        search->add_post_tag(post_id, tag_id);
    }

    auto api::add_related_post(
        std::string_view post_id,
        std::string_view related
    ) -> void {
        TIMBER_FUNC();

        db->create_related_post(post_id, related);
    }

    auto api::add_reply(
        std::string_view parent_id,
        std::string_view content
    ) -> comment {
        TIMBER_FUNC();

        const auto comment = db->create_reply(
            parent_id,
            format_comment(content)
        );

        return {
            comment.id,
            comment.content,
            comment.indent,
            comment.date_created
        };
    }

    auto api::add_source(std::string_view url) -> source {
        constexpr auto host_prefix = std::string_view("www.");

        const auto uri = uri::uri(std::string(url));

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
                const auto object = objects->add(
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
        TIMBER_FUNC();

        const auto formatted = format_tag(name);
        const auto id = db->create_tag(formatted);

        search->add_tag_alias(id, formatted);

        return id;
    }

    auto api::add_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        TIMBER_FUNC();

        const auto formatted = format_tag(alias);
        const auto name = db->create_tag_alias(tag_id, formatted);

        search->add_tag_alias(tag_id, formatted);

        return name;
    }

    auto api::add_tag_source(
        std::string_view tag_id,
        std::string_view url
    ) -> source {
        TIMBER_FUNC();

        const auto src = add_source(url);
        db->create_tag_source(tag_id, src.id);
        return src;
    }

    auto api::delete_post(std::string_view id) -> void {
        TIMBER_FUNC();

        db->delete_post(id);
        search->delete_post(id);
    }

    auto api::delete_post_objects(
        std::string_view post_id,
        const std::vector<std::string>& objects
    ) -> std::string {
        TIMBER_FUNC();

        const auto modified = db->delete_post_objects(post_id, objects);
        search->update_post_date_modified(post_id, modified);
        return modified;
    }

    auto api::delete_post_objects(
        std::string_view post_id,
        std::span<range> ranges
    ) -> std::string {
        TIMBER_FUNC();

        const auto modified = db->delete_post_objects_ranges(post_id, ranges);
        search->update_post_date_modified(post_id, modified);
        return modified;
    }

    auto api::delete_post_tag(
        std::string_view post_id,
        std::string_view tag_id
    ) -> void {
        TIMBER_FUNC();

        db->delete_post_tag(post_id, tag_id);
        search->remove_post_tag(post_id, tag_id);
    }

    auto api::delete_related_post(
        std::string_view post_id,
        std::string_view related
    ) -> void {
        TIMBER_FUNC();

        db->delete_related_post(post_id, related);
    }

    auto api::delete_tag(std::string_view id) -> void {
        TIMBER_FUNC();

        db->delete_tag(id);
        search->delete_tag(id);
    }

    auto api::delete_tag_alias(
        std::string_view tag_id,
        std::string_view alias
    ) -> tag_name {
        TIMBER_FUNC();

        const auto name = db->delete_tag_alias(tag_id, alias);
        search->delete_tag_alias(tag_id, alias);
        return name;
    }

    auto api::delete_tag_source(
        std::string_view tag_id,
        std::string_view source_id
    ) -> void {
        TIMBER_FUNC();

        db->delete_tag_source(tag_id, source_id);
    }

    auto api::get_comments(std::string_view post_id) -> comment_tree {
        TIMBER_FUNC();

        const auto entities = db->read_comments(post_id);
        return build_tree(entities);
    }

    auto api::get_object(std::string_view object_id) -> object {
        TIMBER_FUNC();

        return object(
            db->read_object(object_id),
            objects->meta(object_id),
            get_posts(db->read_object_posts(object_id))
        );
    }

    auto api::get_post(std::string_view id) -> post {
        TIMBER_FUNC();

        auto data = db->read_post(id);

        auto result = post {
            .id = data.id,
            .title = data.title,
            .description = data.description,
            .date_created = data.date_created,
            .date_modified = data.date_modified,
            .posts = get_posts(db->read_related_posts(id)),
            .tags = db->read_post_tags(id)
        };

        auto objects = db->read_post_objects(id);

        for (auto&& obj : objects) {
            auto meta = this->objects->meta(obj.id);
            result.objects.emplace_back(
                std::move(obj),
                std::move(meta)
            );
        }

        return result;
    }

    auto api::get_posts(
        std::vector<repo::db::post_preview>&& posts
    ) -> std::vector<post_preview> {
        auto result = std::vector<post_preview>();

        for (auto&& post : posts) {
            const auto obj = post.preview ?
                post.preview.value().id :
                std::optional<std::string>();

            result.emplace_back(
                std::move(post),
                post.preview ?
                    object_preview(
                        std::move(post.preview.value()),
                        objects->meta(obj.value())
                    ) :
                    std::optional<object_preview>()
            );
        }

        return result;
    }

    auto api::get_posts(
        const post_query& query
    ) -> search_result<post_preview> {
        TIMBER_FUNC();

        const auto result = search->find_posts(query);
        return {
            .total = result.total,
            .hits = get_posts(db->read_posts(result.hits))
        };
    }

    auto api::get_tag(std::string_view id) -> tag {
        TIMBER_FUNC();

        return tag(db->read_tag(id), db->read_tag_sources(id));
    }

    auto api::get_tags(const tag_query& query) -> search_result<tag_preview> {
        TIMBER_FUNC();

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
        TIMBER_FUNC();

        search->update_post_date_modified(
            post_id,
            db->move_post_object(post_id, old_index, new_index)
        );
    }

    auto api::move_post_objects(
        std::string_view post_id,
        const std::vector<std::string>& objects,
        std::optional<std::string> destination
    ) -> std::string {
        TIMBER_FUNC();

        const auto date_modified = db->move_post_objects(
            post_id,
            objects,
            destination
        );

        search->update_post_date_modified(post_id, date_modified);
        return date_modified;
    }

    auto api::prune() -> void {
        TIMBER_FUNC();

        db->prune();

        auto result = fstore::remove_result();
        db->prune_objects([this, &result](auto objects) -> bool {
            try {
                result = this->objects->remove(objects);
                return true;
            }
            catch (const std::runtime_error& ex) {
                TIMBER_ERROR("Object pruning has failed: {}", ex.what());
            }

            return false;
        });

        if (result.objects_removed == 0) {
            TIMBER_INFO("No objects to prune");
            return;
        }

        TIMBER_INFO(
            "Removed {} {} freeing {}",
            result.objects_removed,
            result.objects_removed == 1 ? "object" : "objects",
            data_size(result.space_freed).formatted
        );
    }

    auto api::regenerate_preview(
        std::string_view object_id
    ) -> std::optional<std::string> {
        TIMBER_FUNC();

        const auto metadata = objects->meta(object_id);
        const auto preview = previews.generate_preview(metadata);

        db->update_object_preview(object_id, preview);

        return preview;
    }

    auto api::reindex() -> void {
        TIMBER_FUNC();

        search->delete_indices();
        search->create_indices();

        search->add_posts(db->read_post_search());
        search->add_tags(db->read_tag_text());
    }

    auto api::set_comment_content(
        std::string_view comment_id,
        std::string_view content
    ) -> std::string {
        TIMBER_FUNC();

        const auto formatted = format_comment(content);
        db->update_comment(comment_id, formatted);

        return formatted;
    }

    auto api::set_post_description(
        std::string_view post_id,
        std::string_view description
    ) -> modification<std::optional<std::string>> {
        TIMBER_FUNC();

        const auto update = db->update_post_description(
            post_id,
            format_description(description)
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
        TIMBER_FUNC();

        const auto update = db->update_post_title(post_id, format_title(title));
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
        TIMBER_FUNC();

        return db->update_tag_description(
            tag_id,
            format_description(description)
        );
    }

    auto api::set_tag_name(
        std::string_view tag_id,
        std::string_view new_name
    ) -> tag_name {
        TIMBER_FUNC();

        const auto formatted = format_tag(new_name);
        const auto update  = db->update_tag_name(tag_id, formatted);

        if (update.old_name) {
            search->update_tag_name(
                tag_id,
                update.old_name.value(),
                formatted
            );
        }

        return update.names;
    }
}
