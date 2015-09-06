// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "test/filesystem/mock/storage.h"
#include "gmock/gmock.h"

#include <iostream>

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

const std::string SCHEME{ "mock://" };
const fs::path PATH{ "path" };
const std::string URL = SCHEME + PATH.string();
const fs::file_status STATUS;

TEST(EntryTest, same_url) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    ASSERT_NE(entry, nullptr);
    ASSERT_EQ(entry->url(), URL);
}

TEST(EntryTest, is_dir) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, is_dir(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->is_dir());
    EXPECT_CALL(*storage, is_dir(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->is_dir());
}

TEST(EntryTest, exists) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->exists());
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->exists());
}

TEST(EntryTest, invalidate) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    ASSERT_NE(entry, nullptr);
    entry->invalidate();
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).Times(0);
    EXPECT_CALL(*storage, is_dir(testing::Ref(*entry))).Times(0);
    EXPECT_CALL(*storage, ls(testing::Ref(*entry))).Times(0);
    ASSERT_FALSE(entry->exists());
    ASSERT_THROW(entry->is_dir(), filesystem_error);
    ASSERT_THROW(entry->ls(), filesystem_error);
}

TEST(EntryTest, ls) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    std::string url;
    Entry::entries_t content{
        storage->create("file1", STATUS),
        storage->create("file2", STATUS)};
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, ls(testing::Ref(*entry))).WillOnce(testing::Return(content));
    entry->on_update([&url](const Entry& entry) { url = entry.url(); });
    ASSERT_NO_THROW(entry->ls());
    ASSERT_EQ(entry->content().size(), content.size());
    ASSERT_EQ(entry->url(), url);
}

}  // namespace filesystem
}  // namespace kodama
