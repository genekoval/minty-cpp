#include <minty/core/search.h>

#include <ext/except.h>

namespace minty::core {
    search_engine::search_engine(std::string_view endpoint) :
        endpoint(endpoint)
    {
        INFO() << "Using search engine version " << version();
    }

    auto search_engine::connect() -> client {
        try {
            return client(net::socket(netcore::connect(endpoint)));
        }
        catch (const ext::system_error& ex) {
            ERROR() << ex.what();
            throw std::runtime_error("search service unavailable");
        }
    }

    auto search_engine::add_tag(const search::tag& tag) -> void {
        connect().emit(
            event::add_tag,
            tag
        );
    }

    auto search_engine::find_tags_by_name(
        std::string_view term
    ) -> std::vector<std::string> {
        return connect().send<std::vector<std::string>>(
            event::find_tags_by_name,
            term
        );
    }

    auto search_engine::version() -> std::string {
        return connect().send<std::string>(event::version);
    }
}
