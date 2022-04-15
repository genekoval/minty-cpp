#pragma once

#include <minty/test.h>

#include <fstore/client.h>

namespace minty::core {
    class object_store {
        fstore::bucket* bucket = nullptr;
    public:
        object_store() = default;

        object_store(fstore::bucket& bucket);

        VIRTUAL_DESTRUCTOR(object_store)

        VIRTUAL auto add(
            std::optional<UUID::uuid> part_id,
            std::size_t stream_size,
            std::function<void(fstore::part&&)> pipe
        ) -> fstore::object_meta;

        VIRTUAL auto add(std::string_view path) -> fstore::object_meta;

        VIRTUAL auto get(const UUID::uuid& object_id) -> fstore::blob;

        VIRTUAL auto get(
            const UUID::uuid& object_id,
            std::byte* buffer
        ) -> void;

        VIRTUAL auto meta(const UUID::uuid& object_id) -> fstore::object_meta;

        VIRTUAL auto remove(const UUID::uuid& object_id) -> fstore::object_meta;

        VIRTUAL auto remove(
            std::span<const UUID::uuid> objects
        ) -> fstore::remove_result;
    };
}
