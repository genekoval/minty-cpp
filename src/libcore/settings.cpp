#include <minty/core/settings.h>
#include <minty/error.h>

#include <sstream>
#include <yaml-cpp/yaml.h>

template <typename Optional>
static auto optional(
    const YAML::Node& node,
    const std::string& key
) -> Optional {
    if (auto value = node[key]) {
        return value.as<typename Optional::value_type>();
    }

    return {};
}

namespace YAML {
    using settings = minty::core::settings;

    template <>
    struct convert<settings::s_database> {
        static auto decode(
            const Node& node,
            settings::s_database& database
        ) -> bool {
            const auto& connection = node["connection"];

            if (connection.IsScalar()) {
                database.connection = connection.as<std::string>();
                return true;
            }

            if (!connection.IsMap()) return false;

            auto os = std::ostringstream();
            auto it = connection.begin();
            auto end = connection.end();

            while (it != end) {
                os << it->first << '=' << it->second;
                if (++it != end) os << ' ';
            }

            database.connection = os.str();
            return true;
        }
    };

    template <>
    struct convert<settings::s_fstore> {
        using T = settings::s_fstore;

        static auto decode(const Node& node, T& fstore) -> bool {
            fstore.bucket = node["bucket"].as<decltype(T::bucket)>();
            fstore.connection =
                node["connection"].as<decltype(T::connection)>();
            fstore.proxy = optional<decltype(T::proxy)>(node, "proxy");

            return true;
        }
    };

    template <>
    struct convert<settings::s_fstore::s_proxy> {
        using T = settings::s_fstore::s_proxy;

        static auto decode(const Node& node, T& proxy) -> bool {
            proxy.host = optional<decltype(T::host)>(node, "host");
            proxy.port = node["port"].as<decltype(T::port)>();

            return true;
        }
    };

    template <>
    struct convert<settings::s_log> {
        static auto decode(const Node& node, settings::s_log& log) -> bool {
            if (node["level"]) {
                auto l = node["level"].as<std::string>();
                auto level = timber::parse_level(l);

                if (!level) {
                    throw std::runtime_error("unknown log level: " + l);
                }

                log.level = *level;
            }

            return true;
        }
    };

    template <>
    struct convert<settings> {
        static auto decode(const Node& node, settings& s) -> bool {
            s.connection = node["connection"]
                .as<decltype(settings::connection)>();
            s.database = node["database"]
                .as<decltype(settings::database)>();
            s.fstore = node["fstore"]
                .as<decltype(settings::fstore)>();
            if (node["log"]) s.log = node["log"]
                .as<decltype(settings::log)>();

            return true;
        }
    };
}

namespace minty::core {
    auto settings::encode() const -> std::string {
        auto out = YAML::Emitter();

        out
            << YAML::BeginMap
                << YAML::Key << "connection"
                << YAML::Value << connection
            << YAML::EndMap
            << YAML::Newline

            << YAML::BeginMap
                << YAML::Key << "database"
                << YAML::Value
                    << YAML::BeginMap
                        << YAML::Key << "connection"
                        << YAML::Value << database.connection
                    << YAML::EndMap
            << YAML::EndMap
            << YAML::Newline

            << YAML::BeginMap
                << YAML::Key << "fstore"
                << YAML::Value
                    << YAML::BeginMap
                        << YAML::Key << "bucket"
                        << YAML::Value << fstore.bucket
                        << YAML::Key << "connection"
                        << YAML::Value << fstore.connection
                    << YAML::EndMap
            << YAML::EndMap
            << YAML::Newline;

        return out.c_str();
    }

    auto settings::load(const std::string& text) -> settings {
        return YAML::Load(text).as<settings>();
    }

    auto settings::load_file(const std::string& path) -> settings {
        return YAML::LoadFile(path).as<settings>();
    }
}
