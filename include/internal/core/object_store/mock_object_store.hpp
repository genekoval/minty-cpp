#pragma once

#include "object_store.hpp"

#include <gmock/gmock.h>

namespace minty::test {
    struct bucket : core::bucket {
        MOCK_METHOD(ext::task<fstore::blob>, get, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<>, get, (
            const UUID::uuid& object_id,
            std::byte* buffer
        ), (override));

        MOCK_METHOD(ext::task<fstore::object>, meta, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<fstore::object>, remove, (
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<fstore::remove_result>, remove, (
            std::span<const UUID::uuid> objects
        ), (override));
    };

    struct object_store : core::object_store {
        auto connect() -> ext::task<core::bucket> override {
            co_return bucket();
        }
    };
}
