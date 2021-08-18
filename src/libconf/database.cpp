#include <minty/conf/settings.h>

#include <sstream>

namespace minty::conf {
    auto settings::s_database::s_connection::str() const -> std::string {
        auto os = std::ostringstream();

        auto it = options.begin();
        const auto end = options.end();

        while (it != end) {
            os << it->first << "=" << it->second;
            if (++it != end) os << " ";
        }

        return os.str();
    }
}
