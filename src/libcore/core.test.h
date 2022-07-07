#pragma once

#include <minty/core/api.h>
#include <minty/core/mock_downloader.h>
#include <minty/core/mock_object_store.h>
#include <minty/core/search/mock_search.h>
#include <minty/repo/db/mock_database.h>

#include <gtest/gtest.h>

namespace minty::test {
    const auto trimmed_text = "text";
    const auto whitespace_text = "  \ttext\n  ";
}

class CoreTest : public testing::Test {
protected:
    minty::test::database db;
    minty::test::object_store objects;
    minty::test::downloader downloader;
    minty::test::search_engine search;
    minty::core::api api = minty::core::api(db, objects, downloader, search);
};
