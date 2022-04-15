#include <minty/core/object_store.h>

namespace minty::core {
    object_store::object_store(fstore::bucket& bucket) : bucket(&bucket) {}

    auto object_store::add(
        std::optional<UUID::uuid> part_id,
        std::size_t stream_size,
        std::function<void(fstore::part&&)> pipe
    ) -> fstore::object_meta {
        return bucket->add(part_id, stream_size, pipe);
    }

    auto object_store::add(std::string_view path) -> fstore::object_meta {
        return bucket->add(path);
    }

    auto object_store::get(const UUID::uuid& object_id) -> fstore::blob {
        return bucket->get(object_id);
	}

    auto object_store::get(
        const UUID::uuid& object_id,
        std::byte* buffer
    ) -> void {
        bucket->get(object_id, buffer);
	}

    auto object_store::meta(
        const UUID::uuid& object_id
    ) -> fstore::object_meta {
        return bucket->meta(object_id);
	}

    auto object_store::remove(
        const UUID::uuid& object_id
    ) -> fstore::object_meta {
        return bucket->remove(object_id);
	}

    auto object_store::remove(
        std::span<const UUID::uuid> objects
    ) -> fstore::remove_result {
        return bucket->remove(objects);
	}
}
