#include <minty/core/object_store.h>

namespace minty::core {
    object_store::object_store(fstore::bucket& bucket) : bucket(&bucket) {}

    auto object_store::add(
        std::optional<std::string_view> part_id,
        std::size_t stream_size,
        std::function<void(fstore::part&&)> pipe
    ) -> fstore::object_meta {
        return bucket->add(part_id, stream_size, pipe);
    }

    auto object_store::add(std::string_view path) -> fstore::object_meta {
        return bucket->add(path);
    }

    auto object_store::get(std::string_view object_id) -> fstore::blob {
        return bucket->get(object_id);
	}

    auto object_store::get(
        std::string_view object_id,
        std::byte* buffer
    ) -> void {
        bucket->get(object_id, buffer);
	}

    auto object_store::meta(std::string_view object_id) -> fstore::object_meta {
        return bucket->meta(object_id);
	}

    auto object_store::remove(
        std::string_view object_id
    ) -> fstore::object_meta {
        return bucket->remove(object_id);
	}

    auto object_store::remove(
        std::span<const std::string> objects
    ) -> fstore::remove_result {
        return bucket->remove(objects);
	}
}
