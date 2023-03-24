#include <internal/core/db/connection_wrapper.hpp>

namespace minty::core::db {
    connection_wrapper::connection_wrapper(
        std::shared_ptr<connection>&& inner
    ) :
        inner(std::forward<std::shared_ptr<connection>>(inner))
    {}

    auto connection_wrapper::begin() -> ext::task<pg::transaction> {
        return inner->begin();
    }

    auto connection_wrapper::create_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> ext::task<comment> {
        return inner->create_comment(post_id, content);
    }

    auto connection_wrapper::create_object(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id,
        const std::optional<std::int64_t>& source_id
    ) -> ext::task<> {
        return inner->create_object(object_id, preview_id, source_id);
    }

    auto connection_wrapper::create_object_preview_error(
        const UUID::uuid& object_id,
        std::string_view message
    ) -> ext::task<> {
        return inner->create_object_preview_error(object_id, message);
    }

    auto connection_wrapper::create_post(
        std::string_view title,
        std::string_view description,
        const std::vector<UUID::uuid>& objects,
        const std::vector<UUID::uuid>& tags
    ) -> ext::task<post_search> {
        return inner->create_post(title, description, objects, tags);
    }

    auto connection_wrapper::create_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        std::int16_t position
    ) -> ext::task<time_point> {
        return inner->create_post_objects(post_id, objects, position);
    }

    auto connection_wrapper::create_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        return inner->create_post_tag(post_id, tag_id);
    }

    auto connection_wrapper::create_reply(
        const UUID::uuid& parent_id,
        std::string_view content
    ) -> ext::task<comment> {
        return inner->create_reply(parent_id, content);
    }

    auto connection_wrapper::create_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        return inner->create_related_post(post_id, related);
    }

    auto connection_wrapper::create_site(
        std::string_view scheme,
        std::string_view name,
        std::optional<UUID::uuid> icon
    ) -> ext::task<site> {
        return inner->create_site(scheme, name, icon);
    }

    auto connection_wrapper::create_source(
        std::int64_t site_id,
        std::string_view resource
    ) -> ext::task<source> {
        return inner->create_source(site_id, resource);
    }

    auto connection_wrapper::create_tag(
        std::string_view name
    ) -> ext::task<UUID::uuid> {
        return inner->create_tag(name);
    }

    auto connection_wrapper::create_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        return inner->create_tag_alias(tag_id, alias);
    }

    auto connection_wrapper::create_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        return inner->create_tag_source(tag_id, source_id);
    }

    auto connection_wrapper::delete_object_preview_error(
        const UUID::uuid& object_id
    ) -> ext::task<> {
        return inner->delete_object_preview_error(object_id);
    }

    auto connection_wrapper::delete_post(
        const UUID::uuid& post_id
    ) -> ext::task<> {
        return inner->delete_post(post_id);
    }

    auto connection_wrapper::delete_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects
    ) -> ext::task<decltype(post::date_modified)> {
        return inner->delete_post_objects(post_id, objects);
    }

    auto connection_wrapper::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        return inner->delete_post_tag(post_id, tag_id);
    }

    auto connection_wrapper::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        return inner->delete_related_post(post_id, related);
    }

    auto connection_wrapper::delete_tag(const UUID::uuid& tag_id) -> ext::task<> {
        return inner->delete_tag(tag_id);
    }

    auto connection_wrapper::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        return inner->delete_tag_alias(tag_id, alias);
    }

    auto connection_wrapper::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        return inner->delete_tag_source(tag_id, source_id);
    }

    auto connection_wrapper::move_post_objects(
        const UUID::uuid& post_id,
        const std::vector<UUID::uuid>& objects,
        const std::optional<UUID::uuid>& destination
    ) -> ext::task<time_point> {
        return inner->move_post_objects(post_id, objects, destination);
    }

    auto connection_wrapper::prune() -> ext::task<> {
        return inner->prune();
    }

    auto connection_wrapper::prune_objects() -> ext::task<std::vector<UUID::uuid>> {
        return inner->prune_objects();
    }

    auto connection_wrapper::read_comment(
        const UUID::uuid& comment_id
    ) -> ext::task<comment> {
        return inner->read_comment(comment_id);
    }

    auto connection_wrapper::read_comments(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<comment>> {
        return inner->read_comments(post_id);
    }

    auto connection_wrapper::read_object(
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        return inner->read_object(object_id);
    }

    auto connection_wrapper::read_object_posts(
        const UUID::uuid& object_id
    ) -> ext::task<std::vector<post_preview>> {
        return inner->read_object_posts(object_id);
    }

    auto connection_wrapper::read_object_preview_errors() ->
        ext::task<std::vector<object_error>>
    {
        return inner->read_object_preview_errors();
    }

    auto connection_wrapper::read_objects(
        int batch_size
    ) -> ext::task<pg::portal<object_preview>> {
        return inner->read_objects(batch_size);
    }

    auto connection_wrapper::read_post(const UUID::uuid& post_id) -> ext::task<post> {
        return inner->read_post(post_id);
    }

    auto connection_wrapper::read_posts(
        const std::vector<UUID::uuid>& posts
    ) -> ext::task<std::vector<post_preview>> {
        return inner->read_posts(posts);
    }

    auto connection_wrapper::read_post_objects(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<object_preview>> {
        return inner->read_post_objects(post_id);
    }

    auto connection_wrapper::read_post_search(
        int batch_size
    ) -> ext::task<pg::portal<post_search>> {
        return inner->read_post_search(batch_size);
    }

    auto connection_wrapper::read_post_tags(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<tag_preview>> {
        return inner->read_post_tags(post_id);
    }

    auto connection_wrapper::read_related_posts(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<post_preview>> {
        return inner->read_related_posts(post_id);
    }

    auto connection_wrapper::read_site(
        std::string_view scheme,
        std::string_view host
    ) -> ext::task<std::optional<std::int64_t>> {
        return inner->read_site(scheme, host);
    }

    auto connection_wrapper::read_tag(const UUID::uuid& tag_id) -> ext::task<tag> {
        return inner->read_tag(tag_id);
    }

    auto connection_wrapper::read_tag_previews(
        const std::vector<UUID::uuid>& tags
    ) -> ext::task<std::vector<tag_preview>> {
        return inner->read_tag_previews(tags);
    }

    auto connection_wrapper::read_tag_sources(
        const UUID::uuid& tag_id
    ) -> ext::task<std::vector<source>> {
        return inner->read_tag_sources(tag_id);
    }

    auto connection_wrapper::read_tag_search(
        int batch_size
    ) -> ext::task<pg::portal<tag_search>> {
        return inner->read_tag_search(batch_size);
    }

    auto connection_wrapper::read_total_objects() -> ext::task<std::size_t> {
        return inner->read_total_objects();
    }

    auto connection_wrapper::update_comment(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> ext::task<> {
        return inner->update_comment(comment_id, content);
    }

    auto connection_wrapper::update_object_preview(
        const UUID::uuid& object_id,
        const std::optional<UUID::uuid>& preview_id
    ) -> ext::task<> {
        return inner->update_object_preview(object_id, preview_id);
    }

    auto connection_wrapper::update_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> ext::task<post_update> {
        return inner->update_post_description(post_id, description);
    }

    auto connection_wrapper::update_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> ext::task<post_update> {
        return inner->update_post_title(post_id, title);
    }

    auto connection_wrapper::update_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> ext::task<std::optional<std::string>> {
        return inner->update_tag_description(tag_id, description);
    }

    auto connection_wrapper::update_tag_name(
        const UUID::uuid& tag_id,
        std::string_view name
    ) -> ext::task<tag_name_update> {
        return inner->update_tag_name(tag_id, name);
    }
}
