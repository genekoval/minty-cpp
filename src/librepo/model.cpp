#include <minty/repo/db/db.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    creator::creator(row_iterator& it, transaction& tx) :
        id(read_field<decltype(id)>(it)),
        name(read_field<decltype(name)>(it)),
        aliases(read_array<decltype(aliases)>(it)),
        bio(read_field<decltype(bio)>(it)),
        avatar(read_field<decltype(avatar)>(it)),
        banner(read_field<decltype(banner)>(it)),
        sources(read_entities<decltype(sources)>(it, tx, "read_sources")),
        date_added(read_field<decltype(date_added)>(it))
    {}

    object::object(row_iterator& it, transaction& tx) :
        id(read_field<decltype(id)>(it)),
        preview_id(read_field<decltype(preview_id)>(it)),
        src(read_entity<decltype(src)>(it, tx))
    {}

    site::site(row_iterator& it, transaction& tx) :
        id(read_field<decltype(id)>(it)),
        name(read_field<decltype(name)>(it)),
        homepage(read_field<decltype(homepage)>(it)),
        thumbnail_id(read_field<decltype(thumbnail_id)>(it))
    {}

    source::source(row_iterator& it, transaction& tx) :
        id(read_field<decltype(id)>(it)),
        url(read_field<decltype(url)>(it)),
        website(read_entity<decltype(website)>(it, tx))
    {}

    tag::tag(row_iterator& it, transaction& tx) :
        id(read_field<decltype(id)>(it)),
        name(read_field<decltype(name)>(it)),
        color(read_field<decltype(color)>(it)),
        date_created(read_field<decltype(date_created)>(it))
    {}
}
