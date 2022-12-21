#include "DatabasePostTag.test.hpp"

auto DatabasePostTagTest::tables() -> std::vector<std::string> {
    const auto tables = {"tag"};

    auto base = DatabasePostTest::tables();
    base.insert(base.end(), tables.begin(), tables.end());
    return base;
}
