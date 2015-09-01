// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "test/filesystem/mock/storage.h"
#include "gmock/gmock.h"

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

const std::string SCHEME{ "mock://" };
const std::string URL = SCHEME + "path";
const fs::file_status STATUS;

TEST(EntryTest, url) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry = storage->make(URL, STATUS);
    ASSERT_EQ(entry->url(), URL);
}

TEST(EntryTest, is_dir) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry = storage->make(URL, STATUS);
    EXPECT_CALL(*storage, is_dir(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->is_dir());
    EXPECT_CALL(*storage, is_dir(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->is_dir());
}

TEST(EntryTest, exists) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry = storage->make(URL, STATUS);
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->exists());
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->exists());
}

TEST(EntryTest, invalidate) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry = storage->make(URL, STATUS);
    entry->invalidate();
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).Times(0);
    EXPECT_FALSE(entry->exists());
    EXPECT_THROW(entry->is_dir(), filesystem_error);
}

}  // namespace filesystem
}  // namespace kodama
