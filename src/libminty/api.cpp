#include <minty/api.hpp>
#include <minty/except.hpp>

#include <fcntl.h>
#include <uri/uri>
#include <uuid++/uuid++>

namespace fs = std::filesystem;

namespace zipline {
    template <>
    struct encoder<fstore::file, minty::client_type> {
        static auto encode(
            const fstore::file& file,
            minty::client_type& client
        ) -> ext::task<> {
            return zipline::encode(file, client.inner);
        }
    };
}

namespace minty {
    api::api(netcore::socket&& socket) :
        client(new client_type(
            error_list::thrower(),
            std::forward<netcore::socket>(socket)
        ))
    {}

    auto api::add_comment(
        const UUID::uuid& post_id,
        std::string_view content
    ) -> ext::task<comment_data> {
        co_return co_await client->send<comment_data>(
            event::add_comment,
            post_id,
            content
        );
    }

    auto api::add_object_data(
        const fs::path& path
    ) -> ext::task<object_preview> {
        const auto file = fstore::file {
            .fd = open(path.c_str(), O_RDONLY),
            .size = fs::file_size(path)
        };

        co_return co_await client->send<object_preview>(
            event::add_object_data,
            file
        );
    }

    auto api::add_objects(
        std::span<const std::string_view> arguments
    ) -> ext::task<std::vector<object_preview>> {
        auto result = std::vector<object_preview>();

        for (const auto& arg : arguments) {
            const auto uuid = UUID::parse(arg);
            if (uuid) {
                const auto object = co_await get_object(*uuid);

                auto preview = object_preview();
                preview.id = object.id;
                preview.preview_id = object.preview_id;
                preview.type = object.type;
                preview.subtype = object.subtype;

                result.push_back(std::move(preview));
                continue;
            }

            const auto url = uri::parse(std::string(arg));
            if (url && !url.value().scheme().empty()) {
                const auto objects = co_await add_objects_url(arg);

                for (auto&& obj : objects) {
                    result.push_back(std::move(obj));
                }

                continue;
            }

            result.push_back(co_await add_object_data(arg));
        }

        co_return result;
    }

    auto api::add_objects_url(
        std::string_view url
    ) -> ext::task<std::vector<object_preview>> {
        co_return co_await client->send<std::vector<object_preview>>(
            event::add_objects_url,
            url
        );
    }

    auto api::add_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
        std::int16_t position
    ) -> ext::task<time_point> {
        co_return co_await client->send<time_point>(
            event::add_post_objects,
            post_id,
            objects,
            position
        );
    }

    auto api::add_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client->send<void>(
            event::add_post_tag,
            post_id,
            tag_id
        );
    }

    auto api::add_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client->send<void>(event::add_related_post, post_id, related);
    }

    auto api::add_reply(
        const UUID::uuid& parent_id,
        std::string_view content
    ) -> ext::task<comment_data> {
        co_return co_await client->send<comment_data>(
            event::add_reply,
            parent_id,
            content
        );
    }

    auto api::add_tag(std::string_view name) -> ext::task<UUID::uuid> {
        co_return co_await client->send<UUID::uuid>(
            event::add_tag,
            name
        );
    }

    auto api::add_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        co_return co_await client->send<tag_name>(
            event::add_tag_alias,
            tag_id,
            alias
        );
    }

    auto api::add_tag_source(
        const UUID::uuid& tag_id,
        std::string_view url
    ) -> ext::task<source> {
        co_return co_await client->send<source>(
            event::add_tag_source,
            tag_id,
            url
        );
    }

    auto api::create_post(const UUID::uuid& post_id) -> ext::task<> {
        co_await client->send<void>(event::create_post, post_id);
    }

    auto api::create_post_draft() -> ext::task<UUID::uuid> {
        co_return co_await client->send<UUID::uuid>(event::create_post_draft);
    }

    auto api::delete_post(const UUID::uuid& id) -> ext::task<> {
        co_await client->send<void>(event::delete_post, id);
    }

    auto api::delete_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects
    ) -> ext::task<time_point> {
        co_return co_await client->send<time_point>(
            event::delete_post_objects,
            post_id,
            objects
        );
    }

    auto api::delete_post_tag(
        const UUID::uuid& post_id,
        const UUID::uuid& tag_id
    ) -> ext::task<> {
        co_await client->send<void>(event::delete_post_tag, post_id, tag_id);
    }

    auto api::delete_related_post(
        const UUID::uuid& post_id,
        const UUID::uuid& related
    ) -> ext::task<> {
        co_await client->send<void>(
            event::delete_related_post,
            post_id,
            related
        );
    }

    auto api::delete_tag(const UUID::uuid& id) -> ext::task<> {
        co_await client->send<void>(event::delete_tag, id);
    }

    auto api::delete_tag_alias(
        const UUID::uuid& tag_id,
        std::string_view alias
    ) -> ext::task<tag_name> {
        co_return co_await client->send<tag_name>(
            event::delete_tag_alias,
            tag_id,
            alias
        );
    }

    auto api::delete_tag_source(
        const UUID::uuid& tag_id,
        std::int64_t source_id
    ) -> ext::task<> {
        co_await client->send<void>(
            event::delete_tag_source,
            tag_id,
            source_id
        );
    }

    auto api::get_comment(
        const UUID::uuid& comment_id
    ) -> ext::task<comment> {
        co_return co_await client->send<comment>(
            event::get_comment,
            comment_id
        );
    }

    auto api::get_comments(
        const UUID::uuid& post_id
    ) -> ext::task<std::vector<comment_data>> {
        co_return co_await client->send<std::vector<comment_data>>(
            event::get_comments,
            post_id
        );
    }

    auto api::get_object(const UUID::uuid& object_id) -> ext::task<object> {
        co_return co_await client->send<object>(event::get_object, object_id);
    }

    auto api::get_post(const UUID::uuid& id) -> ext::task<post> {
        co_return co_await client->send<post>(
            event::get_post,
            id
        );
    }

    auto api::get_posts(
        const post_query& query
    ) -> ext::task<search_result<post_preview>> {
        co_return co_await client->send<search_result<post_preview>>(
            event::get_posts,
            query
        );
    }

    auto api::get_server_info() -> ext::task<server_info> {
        co_return co_await client->send<server_info>(event::get_server_info);
    }

    auto api::get_tag(const UUID::uuid& id) -> ext::task<tag> {
        co_return co_await client->send<tag>(
            event::get_tag,
            id
        );
    }

    auto api::get_tags(
        const tag_query& query
    ) -> ext::task<search_result<tag_preview>> {
        co_return co_await client->send<search_result<tag_preview>>(
            event::get_tags,
            query
        );
    }

    auto api::move_post_objects(
        const UUID::uuid& post_id,
        std::span<const UUID::uuid> objects,
        const std::optional<UUID::uuid>& destination
    ) -> ext::task<time_point> {
        co_return co_await client->send<time_point>(
            event::move_post_objects,
            post_id,
            objects,
            destination
        );
    }

    auto api::set_comment_content(
        const UUID::uuid& comment_id,
        std::string_view content
    ) -> ext::task<std::string> {
        co_return co_await client->send<std::string>(
            event::set_comment_content,
            comment_id,
            content
        );
    }

    auto api::set_post_description(
        const UUID::uuid& post_id,
        std::string_view description
    ) -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await client->send<modification<std::optional<std::string>>>(
            event::set_post_description,
            post_id,
            description
        );
    }

    auto api::set_post_title(
        const UUID::uuid& post_id,
        std::string_view title
    ) -> ext::task<modification<std::optional<std::string>>> {
        co_return co_await client->send<modification<std::optional<std::string>>>(
            event::set_post_title,
            post_id,
            title
        );
    }

    auto api::set_tag_description(
        const UUID::uuid& tag_id,
        std::string_view description
    ) -> ext::task<std::optional<std::string>> {
        co_return co_await client->send<std::optional<std::string>>(
            event::set_tag_description,
            tag_id,
            description
        );
    }

    auto api::set_tag_name(
        const UUID::uuid& tag_id,
        std::string_view new_name
    ) -> ext::task<tag_name> {
        co_return co_await client->send<tag_name>(
            event::set_tag_name,
            tag_id,
            new_name
        );
    }
}
