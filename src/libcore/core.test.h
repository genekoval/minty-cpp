#pragma once

#include <minty/core/api.h>
#include <minty/core/downloader.h>
#include <minty/core/search.h>

#include <gtest/gtest.h>

class CoreTest : public testing::Test {
protected:
    minty::repo::db::database db;
    fstore::object_store object_store;
    fstore::bucket bucket;
    minty::core::downloader downloader;
    minty::core::search_engine search;
    minty::core::api api;

    CoreTest();
};
