// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "test/filesystem/mock/storage.h"
#include "test/common/exception.h"

#include "gmock/gmock.h"

#include <iostream>
#include <vector>


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

TEST(EntryTest, equals) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry1 = storage->create(PATH, STATUS);
    auto entry2 = storage->create(PATH, STATUS);
    ASSERT_NE(entry1, nullptr);
    ASSERT_NE(entry2, nullptr);
    ASSERT_EQ(*entry1, *entry2);
}

TEST(EntryTest, is_dir) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, is_dir(testing::_)).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->is_dir());
    EXPECT_CALL(*storage, is_dir(testing::_)).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->is_dir());
}

TEST(EntryTest, exists) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, exists(testing::_)).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->exists());
    EXPECT_CALL(*storage, exists(testing::_)).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->exists());
}

TEST(EntryTest, invalidate) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    ASSERT_NE(entry, nullptr);
    entry->invalidate();
    EXPECT_CALL(*storage, exists(testing::_)).Times(0);
    EXPECT_CALL(*storage, is_dir(testing::_)).Times(0);
    EXPECT_CALL(*storage, ls(testing::_)).Times(0);
    ASSERT_FALSE(entry->exists());
    ASSERT_EX_CODE(entry->is_dir(), filesystem_error, no_such_device);
    ASSERT_EX_CODE(entry->ls(), filesystem_error, no_such_device);
}

TEST(EntryTest, ls_nonexistent) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    EXPECT_CALL(*storage, exists(testing::_)).WillOnce(testing::Return(false));
    ASSERT_EX_CODE(entry->ls(), filesystem_error, no_such_file_or_directory);
}

TEST(EntryTest, ls_invalid_dir) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    EXPECT_CALL(*storage, exists(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*storage, is_dir(testing::_)).WillOnce(testing::Return(false));
    ASSERT_EX_CODE(entry->ls(), filesystem_error, not_a_directory);
}

TEST(EntryTest, ls) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(PATH, STATUS);
    std::string url;
    Entry::entries_t content{
        storage->create("file1", STATUS),
        storage->create("file2", STATUS)};
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, exists(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*storage, is_dir(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*storage, ls(testing::_)).WillOnce(testing::Return(content));
    entry->on_update([&url](const Entry& entry) { url = entry.url(); });
    ASSERT_NO_THROW(entry->ls());
    ASSERT_EQ(entry->url(), url);
    ASSERT_EQ(entry->content().size(), content.size());
}


}  // namespace filesystem
}  // namespace kodama
