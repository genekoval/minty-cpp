#pragma once

#include <minty/core/object_store.h>

#include <gmock/gmock.h>

namespace minty::test {
    struct object_store : core::object_store {
        MOCK_METHOD(fstore::object_meta, add, (
            std::optional<std::string_view> part_id,
            std::size_t stream_size,
            std::function<void(fstore::part&&)> pipe
        ), (override));

        MOCK_METHOD(fstore::object_meta, add, (
            std::string_view path
        ), (override));

        MOCK_METHOD(fstore::blob, get, (
            std::string_view object_id
        ), (override));

        MOCK_METHOD(void, get, (
            std::string_view object_id, std::byte* buffer
        ), (override));

        MOCK_METHOD(fstore::object_meta, meta, (
            std::string_view object_id
        ), (override));

        MOCK_METHOD(fstore::object_meta, remove, (
            std::string_view object_id
        ), (override));

        MOCK_METHOD(fstore::remove_result, remove, (
            std::span<const std::string> objects
        ), (override));
    };
}
