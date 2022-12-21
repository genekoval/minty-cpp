#include <internal/core/object_store/object_store.hpp>

namespace minty::core {
    object_store::object_store(std::string_view endpoint) : client(endpoint) {}

    auto object_store::connect() -> ext::task<bucket> {
        auto connection = co_await client.connect();
        co_return bucket(std::move(connection), bucket_id);
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
        ext::pool_item<fstore::object_store>&& connection,
        const UUID::uuid& id
    ) :
        connection(
            std::forward<ext::pool_item<fstore::object_store>>(connection)
        ),
        id(id)
    {}

    auto bucket::deregister() -> void {
        connection->deregister();
    }

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

    auto bucket::register_scoped() -> netcore::register_guard {
        return connection->register_scoped();
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
