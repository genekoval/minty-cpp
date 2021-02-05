#include <minty/core/api.h>
#include <minty/core/settings.h>
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
    const minty::core::settings& settings,
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
    const auto settings = minty::core::settings::load_file(default_config);
    timber::reporting_level() = settings.log.level;

    INFO() << app.name << " version " << app.version << " starting";

    auto database = minty::repo::db::database(settings.database.connection);

    auto object_store = fstore::object_store(settings.fstore.connection);
    auto bucket_info = object_store.fetch_bucket(settings.fstore.bucket);
    auto bucket = fstore::bucket(bucket_info.id, object_store);

    INFO()
        << "Using bucket `"
        << bucket_info.name
        << "` with "
        << bucket_info.size
        << " objects ("
        << ext::data_size::format(bucket_info.space_used)
        << ")";

    auto api = minty::core::api(database, bucket);
    auto info = build_server_info(settings, app.version, bucket_info.id);

    INFO() << "Object source: " << info.object_source;

    minty::server::listen(settings.connection, info, api, []() {
        INFO() << "Server started. Listening for connections...";
    });
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
