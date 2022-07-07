#pragma once

#include <minty/core/search/search.env.test.h>

class SearchTest : public testing::Test {
protected:
    elastic::elasticsearch& client =
        minty::test::SearchEnvironment::client();

    minty::core::search_engine& search =
        minty::test::SearchEnvironment::engine();
};
