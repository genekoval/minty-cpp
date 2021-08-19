#include "yaml.h"

#include <minty/conf/settings.h>
#include <minty/error.h>

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
                    << Key << "connection"
                    << Value << database.connection.parameters
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

    auto settings::load(std::string_view text) -> settings {
        return YAML::Load(text.data()).as<settings>();
    }

    auto settings::load_file(std::string_view path) -> settings {
        return YAML::LoadFile(path.data()).as<settings>();
    }

    auto initialize(std::string_view path) -> settings {
        auto config = settings::load_file(path);

        timber::reporting_level = config.log.level;

        return config;
    }
}
