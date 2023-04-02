#include <minty/model/visibility.hpp>

namespace minty {
    auto to_string(visibility visibility) -> std::string_view {
        using enum visibility;

        switch (visibility) {
            case draft: return "draft";
            case pub: return "public";
            case invalid: return "<invalid visibility>";
        }
    }
}
