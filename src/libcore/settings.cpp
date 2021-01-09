#include <minty/core/settings.h>
#include <minty/error.h>

#include <sstream>
#include <yaml-cpp/yaml.h>

namespace YAML {
    using settings = minty::core::settings;

    template<>
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

    template<>
    struct convert<settings::s_fstore> {
        static auto decode(
            const Node& node,
            settings::s_fstore& fstore
        ) -> bool {
            fstore.bucket = node["bucket"]
                .as<decltype(settings::s_fstore::bucket)>();
            fstore.connection = node["connection"]
                .as<decltype(settings::s_fstore::connection)>();

            return true;
        }
    };

    template<>
    struct convert<settings> {
        static auto decode(const Node& node, settings& s) -> bool {
            s.connection = node["connection"]
                .as<decltype(settings::connection)>();
            s.database = node["database"]
                .as<decltype(settings::database)>();
            s.fstore = node["fstore"]
                .as<decltype(settings::fstore)>();

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
