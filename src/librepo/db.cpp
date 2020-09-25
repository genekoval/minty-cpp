#include "db.h"

namespace minty::repo::db {
    connection_initializer::connection_initializer(pqxx::connection& c) :
        connection(&c)
    {}

    auto connection_initializer::prepare(
        const std::string& name,
        unsigned int argc
    ) -> void {
        auto os = std::ostringstream();

        os << "SELECT * FROM " << name << '(';

        for (decltype(argc) i = 1; i <= argc; i++) {
            os << '$' << i;
            if (i < argc) os << ", ";
        }

        os << ')';

        connection->prepare(name, os.str());
    }
}
