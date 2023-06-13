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
        const auto body = json({
            {"query", {
                {"match_all", json::object()}
            }}
        }).dump();

        auto req = search->client.delete_by_query({name}, body);
        co_await req.send();
    }

    auto index::create() -> ext::task<> {
        co_await search->client.create_index(name, config()).send();
    }

    auto index::exists() -> ext::task<bool> {
        auto req = search->client.index_exists({name});
        co_return co_await req.send();
    }

    auto index::recreate() -> ext::task<> {
        co_await remove();
        co_await create();
    }

    auto index::refresh() -> ext::task<> {
        TIMBER_FUNC();
        TIMBER_DEBUG("Refreshing index: {}", name);

        auto req = search->client.refresh({name});
        co_await req.send();
    }

    auto index::remove() -> ext::task<> {
        const auto req = search->delete_indices({name});
        co_await req;
    }
}
