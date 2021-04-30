#include <minty/core/model.h>

#include <fmt/format.h>

namespace minty::core {
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
