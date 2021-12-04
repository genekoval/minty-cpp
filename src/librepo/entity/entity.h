#pragma once

#include <minty/repo/db/model.h>

#include <entix/entix>

namespace entix {
    ENTITY(minty::repo::db::comment);
    ENTITY(minty::repo::db::object);
    ENTITY(minty::repo::db::object_preview);
    ENTITY(minty::repo::db::post);
    ENTITY(minty::repo::db::post_preview);
    ENTITY(minty::repo::db::post_search);
    ENTITY(minty::repo::db::post_update);
    ENTITY(minty::repo::db::site);
    ENTITY(minty::repo::db::source);
    ENTITY(minty::repo::db::tag);
    ENTITY(minty::repo::db::tag_name);
    ENTITY(minty::repo::db::tag_name_update);
    ENTITY(minty::repo::db::tag_preview);
    ENTITY(minty::repo::db::tag_text);
}
