#include <minty/repo/db/db.h>

#include <ext/string.h>
#include <regex>

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

    auto parse_error(const pqxx::sql_error& error) -> sql_error {
        const auto message = error.what();
        const auto lines = ext::split(message, "\n");

        return {
            .context = std::string(lines[2].substr(10)),
            .detail = std::string(lines[1].substr(9)),
            .error = std::string(lines[0].substr(8))
        };
    }

    unique_entity_violation::unique_entity_violation(
        std::string_view entity_name,
        const sql_error& error
    ) : unique_entity_violation(entity_name, parse_error(error)) {}

    unique_entity_violation::unique_entity_violation(
        std::string_view entity,
        const std::array<std::string, 2> kv
    ) : minty::unique_entity_violation(
        entity,
        kv[0],
        kv[1]
    ) {}

    unique_entity_violation::unique_entity_violation(
        std::string_view entity_name,
        const pqxx::unique_violation& ex
    ) : unique_entity_violation(entity_name, db::parse_error(ex)) {}

   auto unique_entity_violation::parse_error(
       const sql_error& error
    ) -> std::array<std::string, 2> {
        const auto regex = std::regex(
            R"(\(([a-zA-Z0-9_]+)\)=\((.+)\))"
        );

        auto match = std::smatch();
        std::regex_search(error.detail, match, regex);

        return {
            match[1].str(),
            match[2].str()
        };
    }
}
