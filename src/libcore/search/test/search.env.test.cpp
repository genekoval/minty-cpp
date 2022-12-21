#include <internal/conf/settings.env.test.hpp>
#include <internal/core/search/search.env.test.hpp>

namespace {
    auto config() -> const minty::conf::settings::s_search& {
        return minty::test::SettingsEnvironment::settings().search;
    }
}

namespace minty::test {
    auto SearchEnvironment::client() -> elastic::elasticsearch& {
        static auto instance = elastic::elasticsearch(
            config().node,
            config().auth
        );

        return instance;
    }

    auto SearchEnvironment::engine() -> core::search_engine& {
        static auto instance = core::search_engine(
            config().ns,
            config().node,
            config().auth
        );

        return instance;
    }

    auto SearchEnvironment::SetUp() -> void {
        client();
        engine();
    }
}
