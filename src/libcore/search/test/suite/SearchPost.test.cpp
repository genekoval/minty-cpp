#include "SearchPost.test.hpp"

#include <minty/except.hpp>

#include <date/date.h>

namespace minty::test {
    auto now() -> time_point {
        return std::chrono::time_point_cast<time_point::duration>(
            time_point::clock::now()
        );
    }

    auto parse_date(const std::string& string) -> time_point {
        auto stream = std::istringstream(std::string(string));
        auto result = time_point();

        date::from_stream(stream, "%Y-%m-%d %H:%M:%S%z", result);

        return result;
    }
}

SearchPostTest::SearchPostTest() {
    netcore::run([&]() -> ext::task<> {
        co_await index.refresh();
        co_await index.clear();
    }());
}

auto SearchPostTest::SetUpTestSuite() -> void {
    netcore::run([]() -> ext::task<> {
        auto& search = *SearchEnvironment::get().engine;

        if (!co_await search.post_index.exists()) {
            co_await search.post_index.create();
        }
    }());
}

auto SearchPostTest::add_post() ->
    ext::task<std::reference_wrapper<const post>>
{
    return add_post(cpp);
}

auto SearchPostTest::add_post(
    const UUID::uuid& id
) -> ext::task<std::reference_wrapper<const post>> {
    const auto& post = find_post(id);
    co_await search.add_post(post);
    co_return post;
}

auto SearchPostTest::find_post(const UUID::uuid& id) const -> const post& {
    const auto result = std::find_if(
        posts.begin(),
        posts.end(),
        [id](const auto& post) { return post.id == id; }
    );

    if (result == posts.end()) {
        throw minty::minty_error(
            "post not found: {}",
            id
        );
    }

    return *result;
}

auto SearchPostTest::get_post(
    const UUID::uuid& id
) -> ext::task<elastic::json> {
    co_return co_await client
        .get_doc_source(index, id)
        .send();
}
