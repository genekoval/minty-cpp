#pragma once

#include "model.h"
#include "parser.h"

namespace minty::repo::db {
    ENTITY(comment);
    ENTITY(object);
    ENTITY(object_preview);
    ENTITY(post);
    ENTITY(post_preview);
    ENTITY(post_search);
    ENTITY(post_update);
    ENTITY(site);
    ENTITY(source);
    ENTITY(tag);
    ENTITY(tag_name);
    ENTITY(tag_name_update);
    ENTITY(tag_preview);
    ENTITY(tag_text);
}
