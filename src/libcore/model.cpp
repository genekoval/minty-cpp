#include <minty/core/model.h>

#include <ext/data_size.h>
#include <fmt/format.h>

namespace minty::core {
    constexpr auto decimal_places = 2;

    object::object(
        repo::db::object&& obj,
        fstore::object_meta&& meta,
        std::vector<post_preview>&& posts
    ) :
        id(std::move(meta.id)),
        hash(std::move(meta.hash)),
        size(std::move(meta.size)),
        type(std::move(meta.type)),
        subtype(std::move(meta.subtype)),
        date_added(std::move(meta.date_added)),
        preview_id(std::move(obj.preview_id)),
        src(std::move(obj.src)),
        posts(std::move(posts))
    {}

    object_preview::object_preview(
        repo::db::object_preview&& obj,
        fstore::object_meta&& meta
    ) :
        id(std::move(obj.id)),
        preview_id(std::move(obj.preview_id)),
        type(std::move(meta.type)),
        subtype(std::move(meta.subtype))
    {}

    object_preview::object_preview(
        UUID::uuid&& id,
        std::optional<UUID::uuid>&& preview_id,
        std::string&& type,
        std::string&& subtype
    ) :
        id(std::move(id)),
        preview_id(std::move(preview_id)),
        type(std::move(type)),
        subtype(std::move(subtype))
    {}

    post_preview::post_preview(
        repo::db::post_preview&& p,
        std::optional<object_preview>&& obj
    ) :
        id(std::move(p.id)),
        title(std::move(p.title)),
        preview(std::move(obj)),
        comment_count(std::move(p.comment_count)),
        object_count(std::move(p.object_count)),
        date_created(std::move(p.date_created))
    {}

    data_size::data_size(uintmax_t bytes) :
        bytes(bytes),
        formatted(ext::data_size::format(bytes).str(decimal_places))
    {}

    source::source(const repo::db::source& src) :
        id(src.id),
        url(fmt::format(
            "{}://{}{}",
            src.website.scheme,
            src.website.host,
            src.resource
        )),
        icon(src.website.icon)
    {}

    tag::tag(
        const repo::db::tag& t,
        std::vector<repo::db::source>&& sources
    ) :
        id(t.id),
        name(t.name),
        aliases(t.aliases),
        description(t.description),
        avatar(t.avatar),
        banner(t.banner),
        post_count(t.post_count),
        date_created(t.date_created)
    {
        for (auto&& src : sources) {
            this->sources.emplace_back(std::move(src));
        }
    }
}
