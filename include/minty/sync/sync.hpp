#pragma once

#include <minty/model/object_preview.hpp>

#include <uri/uri>

namespace minty::sync::detail {
    template <typename Repo>
    auto add_objects(
        std::span<const std::string_view> arguments,
        Repo& repo
    ) -> std::vector<object_preview> {
        auto result = std::vector<object_preview>();

        for (const auto& arg : arguments) {
            if (const auto uuid = UUID::parse(arg)) {
                const auto object = repo.get_object(*uuid);

                auto preview = object_preview();
                preview.id = object.id;
                preview.preview_id = object.preview_id;
                preview.type = object.type;
                preview.subtype = object.subtype;

                result.push_back(std::move(preview));
                continue;
            }

            const auto uri = ::uri::parse(std::string(arg));
            if (uri && !uri->scheme().empty() && uri->scheme() != "file") {
                const auto objects = repo.add_objects_url(arg);

                for (auto&& object : objects) {
                    result.push_back(std::move(object));
                }

                continue;
            }

            result.push_back(repo.add_object_data(arg));
        }

        return result;
    }
}
