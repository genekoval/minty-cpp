#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/search.h>
#include <minty/conf/settings.h>
#include <minty/server/server.h>
#include <minty/server/server_info.h>

#include <commline/commline>
#include <ext/data_size.h>
#include <filesystem>
#include <fmt/format.h>
#include <fstore/client.h>
#include <timber/timber>

const auto default_config = std::filesystem::path(CONFDIR) / "minty.yml";

static auto build_server_info(
    const minty::conf::settings& settings,
    std::string_view version,
    std::string_view bucket_id
) -> minty::server::server_info {
    const auto host = [&]() -> std::string {
        if (const auto& proxy = settings.fstore.proxy) {
            return fmt::format(
                "{}:{}",
                proxy->host.value_or("0"),
                proxy->port
            );
        }
        else return settings.fstore.connection;
    }();

    const auto path = fmt::format(
        "{}/{}",
        static_cast<std::underlying_type_t<fstore::event>>(
            fstore::event::get_object
        ),
        bucket_id
    );

    return {
        .object_source = fmt::format("zipline://{}/{}", host, path),
        .version = std::string(version)
    };
}

static auto $main(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    const auto settings = minty::conf::settings::load_file(default_config);
    timber::reporting_level() = settings.log.level;

    INFO() << app.name << " version " << app.version << " starting";

    auto database = minty::repo::db::database(settings.database.connection);

    auto object_store = fstore::object_store(settings.fstore.connection);
    auto bucket_info = object_store.fetch_bucket(settings.fstore.bucket);
    auto bucket = fstore::bucket(bucket_info.id, object_store);

    auto downloader = minty::core::downloader(
        settings.downloader.host,
        settings.downloader.port
    );

    auto search = minty::core::search_engine(settings.search.host);

    INFO()
        << "Using bucket `"
        << bucket_info.name
        << "` with "
        << bucket_info.size
        << " objects ("
        << ext::data_size::format(bucket_info.space_used).str(2)
        << ")";

    auto api = minty::core::api(database, bucket, downloader, search);
    auto info = build_server_info(settings, app.version, bucket_info.id);

    INFO() << "Object source: " << info.object_source;

    minty::server::listen(api, info, settings.server, []() {
        INFO() << "Server started. Listening for connections...";
    });

    INFO() << "Shutting down...";
}

auto main(int argc, const char** argv) -> int {
    auto app = commline::application(
        NAME,
        VERSION,
        DESCRIPTION,
        $main
    );

    return app.run(argc, argv);
}
