#include <internal/conf/settings.env.test.hpp>
#include <internal/core/mock_repo.hpp>
#include <internal/server/server.hpp>

#include <minty/repo.hpp>

#include <gtest/gtest.h>

using namespace ext::literals;
using namespace std::literals;

using minty::core::mock_repo;
using minty::object_preview;
using minty::server::router_type;
using minty::server_info;
using netcore::address_type;
using testing::Return;
using testing::Test;
using UUID::uuid;

namespace {
    constexpr auto tag_name = "Test Tag"sv;

    const minty::time_point time_point =
        std::chrono::time_point_cast<minty::time_point::duration>(
            minty::time_point::clock::now()
        );

    const uuid post_id = "3f1bd4c6-c1ad-4fae-ab1c-a0dd4e13ac7e";
    const uuid tag_id = "1e2478f5-5a29-40b8-8c0e-9ef243f0deb5";

    const auto comment_data = minty::comment_data {
        .id = "6c3b0c1f-fcc8-4c94-89b0-5841ea3d8542",
        .content = "Test Comment",
        .indent = 0,
        .date_created = time_point
    };

    const auto objects = std::vector<object_preview> {
        {
            .id = "19ad00d7-1acc-4995-9b80-c1ecac41f8be",
            .type = "text",
            .subtype = "plain"
        },
        {
            .id = "f2df6242-90ef-4f10-914d-9d4877f8a6a6",
            .preview_id = "4c46149f-0e51-4e3a-a995-8302e967721f",
            .type = "image",
            .subtype = "jpeg"
        },
        {
            .id = "441edcdf-b2be-465d-ba45-47fc73f46f0a",
            .preview_id = "e96efdaf-16c8-45d6-9e8f-6f34cf335490",
            .type = "video",
            .subtype = "mp4"
        }
    };

    const auto source = minty::source {
        .id = 500,
        .url = "https://example.com",
        .icon = "ac053ab3-0d1e-4564-bbaa-a927d45be9da"
    };

    using action = std::function<ext::task<>(minty::repo&)>;

    class server_context {
        router_type router;
        server_info info;
    public:
        server_context(minty::core::repo& repo) :
            router(minty::server::make_router(repo, info))
        {}

        auto close() -> void {
            TIMBER_DEBUG("Test server closed");
        }

        auto connection(netcore::socket&& client) -> ext::task<> {
            auto socket = minty::socket(
                std::forward<netcore::socket>(client),
                8_KiB
            );
            co_await router.route(socket);
        }

        auto listen(const address_type& address) -> void {
            TIMBER_DEBUG(
                "Test server listening for connections on {}",
                address
            );
        }
    };
}

class ClientTest : public Test {
    static auto endpoint() -> const netcore::endpoint& {
        return minty::test::SettingsEnvironment::settings()
            .server
            .front()
            .endpoint;
    }

    auto connect() -> ext::task<minty::repo> {
        co_return minty::repo(co_await netcore::buffered_socket::connect(
            endpoint(),
            8_KiB
        ));
    }

    auto task(action&& f) -> ext::task<> {
        const auto server_task = server.listen(endpoint());

        {
            auto client = co_await connect();
            co_await std::forward<action>(f)(client);
        }

        server.close();
        co_await server_task;
    }
protected:
    mock_repo repo;
    netcore::server<server_context> server;

    ClientTest() : server(repo) {}

    auto connect(action&& f) -> void {
        netcore::run(task(std::forward<action>(f)));
    }
};

#define TEST_VOID(Name, Function, ...) \
    TEST_F(ClientTest, Name) { \
        connect([&](minty::repo& client) -> ext::task<> { \
            EXPECT_CALL(repo, Function(__VA_ARGS__)) \
                .WillOnce(Return(ext::make_task())); \
            co_await client.Function(__VA_ARGS__); \
        }); \
    }

#define TEST_VALUE(Name, Function, Result, ...) \
    TEST_F(ClientTest, Name) { \
        connect([&](minty::repo& client) -> ext::task<> { \
            EXPECT_CALL(repo, Function(__VA_ARGS__)) \
                .WillOnce(Return(ext::make_task(Result))); \
            const auto result = co_await client.Function(__VA_ARGS__); \
            EXPECT_EQ(Result, result); \
        }); \
    }

TEST_VALUE(AddComment, add_comment, comment_data, post_id, comment_data.content)

TEST_VALUE(AddObjectsUrl, add_objects_url, objects, source.url)

TEST_VOID(AddPostTag, add_post_tag, post_id, tag_id)

TEST_VALUE(AddReply, add_reply, comment_data,
    comment_data.id,
    comment_data.content
)

TEST_VALUE(AddTag, add_tag, tag_id, tag_name)

TEST_VALUE(AddTagSource, add_tag_source, source, tag_id, source.url)

TEST_VOID(CreatePost, create_post, post_id)

TEST_VALUE(CreatePostDraft, create_post_draft, post_id)

TEST_VOID(DeletePost, delete_post, post_id)

TEST_VOID(DeletePostTag, delete_post_tag, post_id, tag_id)

TEST_VOID(DeleteTag, delete_tag, tag_id)
