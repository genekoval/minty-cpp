#pragma once

#include <internal/test.hpp>

#include <fstore/fstore>

namespace minty::core {
    class bucket {
        fstore::client::pool::item connection;
        UUID::uuid id;
    public:
        bucket() = default;

        bucket(
            fstore::client::pool::item&& connection,
            const UUID::uuid& id
        );

        bucket(bucket&) = delete;

        bucket(bucket&&) = default;

        VIRTUAL_DESTRUCTOR(bucket)

        auto operator=(const bucket&) -> bucket& = delete;

        auto operator=(bucket&&) -> bucket& = default;

        auto add(
            std::optional<UUID::uuid> part_id,
            std::size_t stream_size,
            const fstore::add_object_fn auto& pipe
        ) -> ext::task<fstore::object> {
            co_return co_await connection->add_object(
                id,
                part_id,
                stream_size,
                pipe
            );
        }

        VIRTUAL auto get(
            const UUID::uuid& object_id
        ) -> ext::task<fstore::blob>;

        VIRTUAL auto get(
            const UUID::uuid& object_id,
            std::byte* buffer
        ) -> ext::task<>;

        VIRTUAL auto meta(
            const UUID::uuid& object_id
        ) -> ext::task<fstore::object>;

        VIRTUAL auto remove(
            const UUID::uuid& object_id
        ) -> ext::task<fstore::object>;

        VIRTUAL auto remove(
            std::span<const UUID::uuid> objects
        ) -> ext::task<fstore::remove_result>;
    };

    class object_store {
        UUID::uuid bucket_id;
        fstore::client client;
    public:
        object_store() = default;

        object_store(std::string_view endpoint);

        VIRTUAL auto connect() -> ext::task<bucket>;

        auto get_bucket_id() const noexcept -> const UUID::uuid&;

        auto init(std::string_view bucket_name) -> ext::task<>;
    };
}
