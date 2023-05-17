#include <internal/core/object_store/object_store.hpp>

namespace minty::core {
    object_store::object_store(std::string_view endpoint) : client(endpoint) {}

    auto object_store::connect() -> ext::task<bucket> {
        co_return bucket(co_await client.connect(), bucket_id);
    }

    auto object_store::get_bucket_id() const noexcept -> const UUID::uuid& {
        return bucket_id;
    }

    auto object_store::init(
            std::string_view bucket_name
        ) -> ext::task<> {
        auto connection = co_await client.connect();
        const auto bucket = co_await connection->fetch_bucket(bucket_name);

        bucket_id = bucket.id;
    }

    bucket::bucket(
        fstore::client::pool::item&& connection,
        const UUID::uuid& id
    ) :
        connection(
            std::forward<fstore::client::pool::item>(connection)
        ),
        id(id)
    {}

    auto bucket::get(
        const UUID::uuid& object_id
    ) -> ext::task<fstore::blob> {
        co_return co_await connection->get_object(id, object_id);
    }

    auto bucket::get(
        const UUID::uuid& object_id,
        std::byte* buffer
    ) -> ext::task<> {
        co_await connection->get_object(id, object_id, buffer);
    }

    auto bucket::meta(
        const UUID::uuid& object_id
    ) -> ext::task<fstore::object> {
        co_return co_await connection->get_object_metadata(id, object_id);
    }

    auto bucket::remove(
        const UUID::uuid& object_id
    ) -> ext::task<fstore::object> {
        co_return co_await connection->remove_object(id, object_id);
    }

    auto bucket::remove(
        std::span<const UUID::uuid> objects
    ) -> ext::task<fstore::remove_result> {
        co_return co_await connection->remove_objects(id, objects);
    }
}
