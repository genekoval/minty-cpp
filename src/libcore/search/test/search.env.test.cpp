#include <internal/conf/settings.env.test.hpp>
#include <internal/core/http.env.test.hpp>
#include <internal/core/search/search.env.test.hpp>

SearchEnvironment* SearchEnvironment::instance = nullptr;

auto SearchEnvironment::get() -> SearchEnvironment& {
    if (instance) return *instance;
    throw std::runtime_error("Search Environment instance is missing");
}

auto SearchEnvironment::SetUp() -> void {
    const auto& search = SettingsEnvironment::get().settings.search;

    client = std::unique_ptr<elastic::elasticsearch>(new elastic::elasticsearch(
        *HttpEnvironment::get().session,
        search.node,
        search.auth
    ));

    engine = std::unique_ptr<minty::core::search_engine>(
        new minty::core::search_engine(
            *HttpEnvironment::get().session,
            search.ns,
            search.node,
            search.auth
        )
    );

    instance = this;
}

auto SearchEnvironment::TearDown() -> void { instance = nullptr; }
