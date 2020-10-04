#pragma once

#include <ext/string.h>
#include <sstream>
#include <stdexcept>

namespace minty {
    class minty_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    class unique_entity_violation : public minty_error {
        static auto create_message(
            std::string_view entity,
            std::string_view identifier
        ) -> std::string {
            auto os = std::ostringstream();

            os << "cannot create "
            << entity << " "
            << ext::quote(identifier) << ": "
            << entity << " exists";

            return os.str();
        }
    public:
        unique_entity_violation(
            std::string_view entity,
            std::string_view identifier
        ) : minty_error(create_message(entity, identifier))
        {}
    };
}
