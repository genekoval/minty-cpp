#include <minty/core/search/search.h>

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

    auto index::clear() -> void {
        search->client
            .delete_by_query({name}, json({
                {"query", {
                    {"match_all", json::object()}
                }}
            }).dump())
            .send();
    }

    auto index::create() -> void {
        search->client.create_index(name, config()).send();
    }

    auto index::exists() -> bool {
        return search->client.index_exists({name}).send();
    }

    auto index::refresh() -> void {
        TIMBER_FUNC();
        TIMBER_DEBUG("Refreshing index: {}", name);
        search->client.refresh({name}).send();
    }

    auto index::remove() -> void {
        search->delete_indices({name});
    }
}
