#include <minty/core/api.h>
#include <minty/core/settings.h>
#include <minty/server/server.h>

#include <commline/commline>
#include <ext/data_size.h>
#include <filesystem>
#include <fstore/client.h>
#include <timber/timber>

const auto default_config = std::filesystem::path(CONFDIR) / "minty.yml";

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

    minty::server::listen(api, settings.connection, []() {
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
