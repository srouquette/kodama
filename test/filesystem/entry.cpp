// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "filesystem/property_status.h"
#include "test/filesystem/mock/storage.h"
#include "gmock/gmock.h"

#include <iostream>

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

const std::string SCHEME{ "mock://" };
const std::string URL = SCHEME + "path";

TEST(EntryTest, unknown_property) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    ASSERT_THROW(entry->get_property<PropertyStatus>(), std::out_of_range);
}

TEST(EntryTest, set_property_with_nullptr) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    ASSERT_THROW(entry->set_property(nullptr), std::bad_typeid);
}

TEST(EntryTest, set_property_with_null_property) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    auto property = std::make_unique<PropertyStatus>(fs::file_status{});
    property.reset(nullptr);
    ASSERT_THROW(entry->set_property(std::move(property)), std::bad_typeid);
}

TEST(EntryTest, get_property) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    entry->set_property(std::make_unique<PropertyStatus>(fs::file_status{}));
    ASSERT_NO_THROW(entry->get_property<PropertyStatus>());
}

TEST(EntryTest, same_url) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    ASSERT_EQ(entry->url(), URL);
}

TEST(EntryTest, is_dir) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, is_dir(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->is_dir());
    EXPECT_CALL(*storage, is_dir(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->is_dir());
}

TEST(EntryTest, exists) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_FALSE(entry->exists());
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_TRUE(entry->exists());
}

TEST(EntryTest, shared_lock) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_THROW(entry->shared_lock(), filesystem_error);
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_NO_THROW(entry->shared_lock());
}

TEST(EntryTest, unique_lock) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(false));
    ASSERT_THROW(entry->unique_lock(), filesystem_error);
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).WillOnce(testing::Return(true));
    ASSERT_NO_THROW(entry->unique_lock());
}

TEST(EntryTest, invalidate) {
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    entry->invalidate();
    EXPECT_CALL(*storage, exists(testing::Ref(*entry))).Times(0);
    ASSERT_FALSE(entry->exists());
    ASSERT_THROW(entry->is_dir(), filesystem_error);
}

}  // namespace filesystem
}  // namespace kodama
