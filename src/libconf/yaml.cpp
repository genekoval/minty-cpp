#include "yaml.h"

namespace c = conftools;

using settings = minty::conf::settings;

namespace YAML {
    DECODE(
        settings,
        c::required("daemon", &settings::daemon),
        c::required("database", &settings::database),
        c::required("downloader", &settings::downloader),
        c::required("fstore", &settings::fstore),
        c::optional("log", &settings::log),
        c::required("search", &settings::search),
        c::required("server", &settings::server)
    );

    DECODE(
        settings::s_daemon,
        c::required("group", &settings::s_daemon::group),
        c::required("pidfile", &settings::s_daemon::pidfile),
        c::required("user", &settings::s_daemon::user)
    );

    DECODE(
        settings::s_database,
        c::optional("client", &settings::s_database::client),
        c::required("connection", &settings::s_database::connection),
        c::optional("dump ", &settings::s_database::dump),
        c::optional("restore", &settings::s_database::restore)
    );

    DECODE(
        settings::s_downloader,
        c::required("host", &settings::s_downloader::host),
        c::required("port", &settings::s_downloader::port)
    );

    DECODE(
        settings::s_fstore,
        c::required("bucket", &settings::s_fstore::bucket),
        c::required("connection", &settings::s_fstore::connection),
        c::required("proxy", &settings::s_fstore::proxy)
    );

    DECODE(
        settings::s_fstore::s_proxy,
        c::optional("host", &settings::s_fstore::s_proxy::host),
        c::required("port", &settings::s_fstore::s_proxy::port)
    );

    DECODE(
        settings::s_log,
        c::optional("level", &settings::s_log::level)
    );

    DECODE(
        settings::s_search,
        c::required("host", &settings::s_search::host)
    );
}
