#include "SearchPost.test.h"

#include <minty/error.h>

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
    index.refresh();
    index.clear();
}

auto SearchPostTest::SetUpTestSuite() -> void {
    auto& search = minty::test::SearchEnvironment::engine();

    if (!search.post_index.exists()) {
        search.post_index.create();
    }
}

auto SearchPostTest::add_post() -> const post& {
    return add_post(cpp);
}

auto SearchPostTest::add_post(const UUID::uuid& id) -> const post& {
    const auto& post = find_post(id);
    search.add_post(post);
    return post;
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

auto SearchPostTest::get_post(const UUID::uuid& id) -> elastic::json {
    return client
        .get_doc_source(index, id)
        .send();
}
