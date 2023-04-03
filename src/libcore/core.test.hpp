#pragma once

#include <internal/core/db/mock_connection.hpp>
#include <internal/core/db/test_database.hpp>
#include <internal/core/downloader/mock_downloader.hpp>
#include <internal/core/object_store/mock_object_store.hpp>
#include <internal/core/repo.hpp>
#include <internal/core/search/mock_search.hpp>

#include <gtest/gtest.h>

namespace minty::test {
    const auto trimmed_text = "text";
    const auto whitespace_text = "  \ttext\n  ";
}

class CoreTest : public testing::Test {
protected:
    std::shared_ptr<minty::core::db::mock_connection> db;
    minty::core::db::test_database database;
    minty::test::object_store objects;
    minty::test::downloader downloader;
    minty::test::search_engine search;
    minty::core::repo repo;

    CoreTest();
};
