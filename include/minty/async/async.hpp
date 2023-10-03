#pragma once

#include <minty/except.hpp>
#include <minty/model/object_preview.hpp>

#include <uri/uri>

namespace minty::async::detail {
    template <typename Repo>
    auto add_objects(std::span<const std::string_view> arguments, Repo& repo)
        -> ext::task<std::vector<object_preview>> {
        auto result = std::vector<object_preview>();

        for (const auto& arg : arguments) {
            if (const auto uuid = UUID::parse(arg)) {
                const auto object = co_await repo.get_object(*uuid);

                if (!object) throw not_found("Object '{}' not found", *uuid);

                auto preview = object_preview();
                preview.id = object->id;
                preview.preview_id = object->preview_id;
                preview.type = object->type;
                preview.subtype = object->subtype;

                result.push_back(std::move(preview));
                continue;
            }

            const auto uri = ::uri::parse(std::string(arg));
            if (uri && !uri->scheme().empty() && uri->scheme() != "file") {
                const auto objects = co_await repo.add_objects_url(arg);

                for (auto&& object : objects) {
                    result.push_back(std::move(object));
                }

                continue;
            }

            result.push_back(co_await repo.add_object_data(arg));
        }

        co_return result;
    }
}
