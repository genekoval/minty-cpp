#include "../model.h"

namespace minty::cli::output {
    auto human_readable<repo::db::tag_preview>::print(
        std::FILE* f,
        const repo::db::tag_preview& tag
    ) -> void {
        fmt::print(f, fmt::emphasis::bold, "{}\n", tag.name);
        fmt::print(f, style::indent);
        fmt::print(f, style::secondary, "ID: {}\n", tag.id);
    }
}
