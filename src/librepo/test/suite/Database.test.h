#pragma once

#include <minty/repo/db/db.env.test.h>

#include <entix/entix>

class DatabaseTest : public testing::Test {
    pqxx::connection& connection =
        minty::test::DatabaseEnvironment::connection();
protected:
    minty::repo::db::database& database =
        minty::test::DatabaseEnvironment::database();

    virtual auto SetUp() -> void;

    auto count(std::string_view table) -> int;

    template <typename ...Args>
    auto exec0(std::string_view query, Args&&... args) -> void {
        const auto q = pqxx::zview(query);
        auto tx = pqxx::nontransaction(connection);

        if constexpr (sizeof...(args) > 0) tx.exec_params0(q, args...);
        else tx.exec0(q);
    }

    template <typename Result, typename ...Args>
    auto exec1(std::string_view query, Args&&... args) -> Result {
        const auto q = pqxx::zview(query);
        auto tx = pqxx::nontransaction(connection);

        auto row = pqxx::row();

        if constexpr (sizeof...(args) > 0) row = tx.exec_params1(q, args...);
        else row = tx.exec1(q);

        auto it = row.begin();
        return entix::field_parser<Result>::read(it);
    }

    template <typename Result, typename ...Args>
    auto exec(std::string_view query, Args&&... args) -> std::vector<Result> {
        const auto q = pqxx::zview(query);
        auto tx = pqxx::nontransaction(connection);

        auto rows = pqxx::result();

        if constexpr (sizeof...(args) > 0) rows = tx.exec_params(q, args...);
        else rows = tx.exec(q);

        auto result = std::vector<Result>();

        for (const auto& row : rows) {
            auto it = row.begin();
            result.push_back(entix::field_parser<Result>::read(it));
        }

        return result;
    }

    virtual auto tables() -> std::vector<std::string> = 0;

    auto truncate(std::string_view table) -> void;

    auto truncate_tables() -> void;
};
