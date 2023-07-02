#include <internal/core/search/search.hpp>

#include <fmt/core.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace minty::core {
    index::index() : config(nullptr), search(nullptr) {}

    index::index(
        search_engine* search,
        std::string_view ns,
        std::string_view name,
        config_type* config
    ) :
        config(config),
        search(search),
        name(fmt::format("{}-{}", ns, name))
    {}

    index::operator std::string_view() const noexcept {
        return name;
    }

    auto index::clear() -> ext::task<> {
        co_await search->client
            .delete_by_query({name}, json({
                {"query", {
                    {"match_all", json::object()}
                }}
            }).dump())
            .send();
    }

    auto index::create() -> ext::task<> {
        co_await search->client.create_index(name, config()).send();
    }

    auto index::exists() -> ext::task<bool> {
        co_return co_await search->client.index_exists({name}).send();
    }

    auto index::recreate() -> ext::task<> {
        co_await remove();
        co_await create();
    }

    auto index::refresh() -> ext::task<> {
        TIMBER_FUNC();
        TIMBER_DEBUG("Refreshing index: {}", name);

        co_await search->client.refresh({name}).send();
    }

    auto index::remove() -> ext::task<> {
        co_await search->delete_indices({name});
    }
}
