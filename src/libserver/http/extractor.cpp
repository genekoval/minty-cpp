#include <internal/server/http/extractor.hpp>

namespace http::server::extractor {
    auto data<std::vector<UUID::uuid>>::read(
        request& request
    ) -> ext::task<std::vector<UUID::uuid>> {
        request.expect_content_type(media::utf8_text());

        const auto body = co_await data<std::string>::read(request);

        auto result = std::vector<UUID::uuid>();

        for (const auto& line : ext::string_range(body, "\n")) {
            const auto trimmed = ext::trim(line);
            if (!trimmed.empty()) result.emplace_back(trimmed);
        }

        co_return result;
    }
}
