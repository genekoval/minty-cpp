#pragma once

#include <internal/core/search/search.env.test.hpp>

class SearchTest : public testing::Test {
protected:
    elastic::elasticsearch& client = *SearchEnvironment::get().client;
    minty::core::search_engine& search = *SearchEnvironment::get().engine;
};
