#include <minty/repo/db/db.h>
#include <minty/repo/db/model.h>

namespace minty::repo::db {
    object::object(pqxx::row::const_iterator& it) :
        id(read_field<decltype(id)>(it)),
        preview_id(read_field<decltype(preview_id)>(it)),
        src(read_entity<decltype(src)>(it))
    {}

    site::site(pqxx::row::const_iterator& it) :
        id(read_field<decltype(id)>(it)),
        name(read_field<decltype(name)>(it)),
        homepage(read_field<decltype(homepage)>(it)),
        thumbnail_id(read_field<decltype(thumbnail_id)>(it))
    {}

    source::source(pqxx::row::const_iterator& it) :
        id(read_field<decltype(id)>(it)),
        url(read_field<decltype(url)>(it)),
        website(read_entity<decltype(website)>(it))
    {}

    tag::tag(pqxx::row::const_iterator& it) :
        id(read_field<decltype(id)>(it)),
        name(read_field<decltype(name)>(it)),
        color(read_field<decltype(color)>(it)),
        date_created(read_field<decltype(date_created)>(it))
    {}
}
