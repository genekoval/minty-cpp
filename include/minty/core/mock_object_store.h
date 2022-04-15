#pragma once

#include <minty/core/object_store.h>

#include <gmock/gmock.h>

namespace minty::test {
    struct object_store : core::object_store {
        MOCK_METHOD(fstore::object_meta, add, (
            std::optional<UUID::uuid> part_id,
            std::size_t stream_size,
            std::function<void(fstore::part&&)> pipe
        ), (override));

        MOCK_METHOD(fstore::object_meta, add, (
            std::string_view path
        ), (override));

        MOCK_METHOD(fstore::blob, get, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(void, get, (
            const UUID::uuid& object_id,
            std::byte* buffer
        ), (override));

        MOCK_METHOD(fstore::object_meta, meta, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(fstore::object_meta, remove, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(fstore::remove_result, remove, (
            std::span<const UUID::uuid> objects
        ), (override));
    };
}
