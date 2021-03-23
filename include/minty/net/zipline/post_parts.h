#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>

namespace minty::net {
    template <typename T>
    using sequence_pair = std::pair<unsigned int, T>;

    template <typename T>
    using object_source_list = std::vector<sequence_pair<T>>;

    struct post_parts {
        decltype(core::post_parts::title) title;
        decltype(core::post_parts::description) description;
        decltype(core::post_parts::creators) creators;
        decltype(core::post_parts::tags) tags;
        object_source_list<std::string> files;
        object_source_list<std::string> urls;
        object_source_list<data_stream> blobs;
    };
}
