#include <commline/commline>

namespace minty::cli::sub::errors {
    auto preview(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
