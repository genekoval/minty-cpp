#pragma once

#include <minty/core/model.h>
#include <minty/net/zipline/protocol.h>
#include <minty/server/server_info.h>

#include <ext/pool>
#include <netcore/netcore>
#include <zipline/zipline>

namespace minty {
    class api {
        std::unique_ptr<net::client_type> client;
    public:
        api() = default;

        api(netcore::socket&& socket);

        auto add_comment(
            const UUID::uuid& post_id,
            std::string_view content
        ) -> ext::task<core::comment>;

        auto add_object_data(
            const std::filesystem::path& path
        ) -> ext::task<core::object_preview>;

        auto add_objects(
            std::span<const std::string_view> arguments
        ) -> ext::task<std::vector<core::object_preview>>;

        auto add_objects_url(
            std::string_view url
        ) -> ext::task<std::vector<core::object_preview>>;

        auto add_post(const core::post_parts& parts) -> ext::task<UUID::uuid>;

        auto add_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            std::int16_t position
        ) -> ext::task<decltype(core::post::date_modified)>;

        auto add_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        auto add_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<>;

        auto add_reply(
            const UUID::uuid& parent_id,
            std::string_view content
        ) -> ext::task<core::comment>;

        auto add_tag(std::string_view name) -> ext::task<UUID::uuid>;

        auto add_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<core::tag_name>;

        auto add_tag_source(
            const UUID::uuid& tag_id,
            std::string_view url
        ) -> ext::task<core::source>;

        auto delete_post(const UUID::uuid& id) -> ext::task<>;

        auto delete_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects
        ) -> ext::task<decltype(core::post::date_modified)>;

        auto delete_post_tag(
            const UUID::uuid& post_id,
            const UUID::uuid& tag_id
        ) -> ext::task<>;

        auto delete_related_post(
            const UUID::uuid& post_id,
            const UUID::uuid& related
        ) -> ext::task<>;

        auto delete_tag(const UUID::uuid& id) -> ext::task<>;

        auto delete_tag_alias(
            const UUID::uuid& tag_id,
            std::string_view alias
        ) -> ext::task<core::tag_name>;

        auto delete_tag_source(
            const UUID::uuid& tag_id,
            std::string_view source_id
        ) -> ext::task<>;

        auto get_comment(const UUID::uuid& comment_id) -> ext::task<core::comment_detail>;

        auto get_comments(
            const UUID::uuid& post_id
        ) -> ext::task<std::vector<core::comment>>;

        auto get_object(const UUID::uuid& object_id) -> ext::task<core::object>;

        auto get_post(const UUID::uuid& id) -> ext::task<core::post>;

        auto get_posts(
            const core::post_query& query
        ) -> ext::task<core::search_result<core::post_preview>>;

        auto get_server_info() -> ext::task<server::server_info>;

        auto get_tag(const UUID::uuid& id) -> ext::task<core::tag>;

        auto get_tags(
            const core::tag_query& query
        ) -> ext::task<core::search_result<core::tag_preview>>;

        auto move_post_objects(
            const UUID::uuid& post_id,
            std::span<const UUID::uuid> objects,
            const std::optional<UUID::uuid>& destination
        ) -> ext::task<decltype(core::post::date_modified)>;

        auto set_comment_content(
            const UUID::uuid& comment_id,
            std::string_view content
        ) -> ext::task<std::string>;

        auto set_post_description(
            const UUID::uuid& post_id,
            std::string_view description
        ) -> ext::task<core::modification<std::optional<std::string>>>;

        auto set_post_title(
            const UUID::uuid& post_id,
            std::string_view title
        ) -> ext::task<core::modification<std::optional<std::string>>>;

        auto set_tag_description(
            const UUID::uuid& tag_id,
            std::string_view description
        ) -> ext::task<std::optional<std::string>>;

        auto set_tag_name(
            const UUID::uuid& tag_id,
            std::string_view new_name
        ) -> ext::task<core::tag_name>;
    };

    class client {
        struct domain {
            std::string path;
        };

        struct network {
            std::string host;
            std::string port;
        };

        using socket_type = std::variant<std::monostate, domain, network>;

        static auto parse_endpoint(std::string_view endpoint) -> socket_type;

        class provider {
            socket_type endpoint;
        public:
            provider();

            provider(socket_type&& endpoint);

            auto provide() -> ext::task<api>;
        };

        ext::async_pool<api, provider> pool;
        fstore::client object_client;
        UUID::uuid bucket_id;
    public:
        client(
            std::string_view endpoint,
            std::string_view object_host
        );

        auto bucket(
            fstore::object_store& object_store
        ) -> ext::task<fstore::bucket>;

        auto connect() -> ext::task<ext::pool_item<api>>;

        auto object_store() -> ext::task<ext::pool_item<fstore::object_store>>;
    };
}
