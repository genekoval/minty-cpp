#include <minty/conf/settings.h>
#include <minty/error.h>

#include <conftools/yaml.h>
#include <sstream>

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
    using settings = minty::conf::settings;

    template <>
    struct convert<settings::s_daemon> {
        using T = settings::s_daemon;

        static auto decode(const Node& node, T& daemon) -> bool {
            daemon.group = node["group"].as<decltype(T::group)>();
            daemon.pidfile = node["pidfile"].as<decltype(T::pidfile)>();
            daemon.user = node["user"].as<decltype(T::user)>();

            return true;
        }
    };

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
    struct convert<settings::s_downloader> {
        using T = settings::s_downloader;

        static auto decode(const Node& node, T& harvest) -> bool {
            harvest.host = node["host"].as<decltype(T::host)>();
            harvest.port = node["port"].as<decltype(T::port)>();

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
    struct convert<settings::s_search> {
        using T = settings::s_search;

        static auto decode(const Node& node, T& search) -> bool {
            search.host = node["host"].as<decltype(T::host)>();

            return true;
        }
    };

    template <>
    struct convert<settings> {
        static auto decode(const Node& node, settings& s) -> bool {
            s.daemon = node["daemon"]
                .as<decltype(settings::daemon)>();
            s.database = node["database"]
                .as<decltype(settings::database)>();
            s.downloader= node["downloader"]
                .as<decltype(settings::downloader)>();
            s.fstore = node["fstore"]
                .as<decltype(settings::fstore)>();
            if (node["log"]) s.log = node["log"]
                .as<decltype(settings::log)>();
            s.search = node["search"]
                .as<decltype(settings::search)>();
            s.server = node["server"]
                .as<netcore::unix_socket>();

            return true;
        }
    };
}

namespace minty::conf {
    auto settings::encode() const -> std::string {
        using namespace YAML;

        auto out = Emitter();

        out
            << BeginMap
                << Key << "server" << Value << BeginMap
                    << Key << "path" << Value << server.path
                << EndMap
            << EndMap
            << Newline

            << BeginMap
                << Key << "database" << Value << BeginMap
                    << Key << "connection" << Value << database.connection
                << EndMap
            << EndMap
            << Newline

            << BeginMap
                << Key << "downloader" << Value << BeginMap
                    << Key << "host" << Value << downloader.host
                    << Key << "port" << Value << downloader.port
                << EndMap
            << EndMap
            << Newline

            << BeginMap
                << Key << "fstore" << Value << BeginMap
                    << Key << "bucket" << Value << fstore.bucket
                    << Key << "connection" << Value << fstore.connection
                << EndMap
            << EndMap
            << Newline;

        return out.c_str();
    }

    auto settings::load(const std::string& text) -> settings {
        return YAML::Load(text).as<settings>();
    }

    auto settings::load_file(const std::string& path) -> settings {
        return YAML::LoadFile(path).as<settings>();
    }
}
