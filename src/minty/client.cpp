#include "client.h"
#include "output.h"

#include <ext/string.h>
#include <iostream>

namespace fs = std::filesystem;

namespace minty::cli {
    auto add_object(minty::api& api, std::string_view object) -> std::string {
        const auto path = fs::canonical(object);
        return api.add_object_local(path.string());
    }

    auto print_post(
        minty::api& api,
        std::string_view id,
        std::optional<std::string_view> path
    ) -> void {
        cli::print(api.get_post(id), path);
    }

    auto print_tag(
        minty::api& api,
        std::string_view id,
        std::optional<std::string_view> path
    ) -> void {
        cli::print(api.get_tag(id), path);
    }

    auto print_yaml(
        const YAML::Emitter& emitter,
        std::optional<std::string_view> path
    ) -> void {
        const auto* str = emitter.c_str();

        if (!path) {
            std::cout << str << std::endl;
            return;
        }

        auto node = YAML::Load(str);

        for (const auto key : ext::string_range(*path, ".")) {
            const auto k = std::string(key);
            node = node[k];

            if (!node) {
                throw std::runtime_error(
                    "key (" + k + ") does not exist in path"
                );
            }
        }

        const auto result = node.as<std::string>();
        std::cout << result;

        if (!result.ends_with("\n")) std::cout << std::endl;
    }
}
