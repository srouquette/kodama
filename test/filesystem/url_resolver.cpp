// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/url_resolver.h"
#include "filesystem/exception.h"
#include "test/common/exception.h"
#include "test/filesystem/mock/storage.h"

#include "gmock/gmock.h"

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

const std::string SCHEME{ "mock://" };
const std::string PATH{ "path" };
const std::string URL{ SCHEME + PATH };
const fs::file_status STATUS;

TEST(UrlResolverTest, add_storage) {
    UrlResolver resolver;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    ASSERT_NO_THROW(resolver.add(storage));
}

TEST(UrlResolverTest, add_storage_with_existing_scheme) {
    UrlResolver resolver;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    ASSERT_NO_THROW(resolver.add(storage));
    ASSERT_NO_THROW(resolver.add(storage));
}

TEST(UrlResolverTest, resolve_without_storage) {
    UrlResolver resolver;
    ASSERT_EX_CODE(resolver.resolve(URL), filesystem_error, no_such_file_or_directory);
}

TEST(UrlResolverTest, cannot_resolve_url) {
    UrlResolver resolver;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    EXPECT_CALL(*storage, resolve(URL)).Times(2).WillRepeatedly(testing::Return(nullptr));
    ASSERT_NO_THROW(resolver.add(storage));
    ASSERT_EX_CODE(resolver.resolve(URL), filesystem_error, no_such_file_or_directory);
}

TEST(UrlResolverTest, can_resolve_url) {
    UrlResolver resolver;
    entry_ptr_t result;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(URL, STATUS);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, resolve(URL)).WillOnce(testing::Return(entry));
    ASSERT_NO_THROW(resolver.add(storage));
    ASSERT_NO_THROW(result = resolver.resolve(URL));
    ASSERT_EQ(result, entry);
}

TEST(UrlResolverTest, can_resolve_url_without_scheme) {
    UrlResolver resolver;
    entry_ptr_t result;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry   = storage->create(URL, STATUS);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, resolve(PATH)).WillOnce(testing::Return(entry));
    ASSERT_NO_THROW(resolver.add(storage));
    ASSERT_NO_THROW(result = resolver.resolve(PATH));
    ASSERT_EQ(result, entry);
}

TEST(UrlResolverTest, resolve_url_with_appropriate_storage) {
    UrlResolver resolver;
    entry_ptr_t result;
    const std::string good_scheme{ "good://" };
    const std::string bad_scheme{ "bad://" };
    const std::string url = good_scheme + PATH;
    auto good_storage = std::make_shared<MockStorage>(good_scheme);
    auto bad_storage  = std::make_shared<MockStorage>(bad_scheme);
    auto entry = good_storage->create(good_scheme, STATUS);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*bad_storage, resolve(url)).Times(0);
    EXPECT_CALL(*good_storage, resolve(url)).WillOnce(testing::Return(entry));
    ASSERT_NO_THROW(resolver.add(bad_storage));
    ASSERT_NO_THROW(resolver.add(good_storage));
    ASSERT_NO_THROW(result = resolver.resolve(url));
    ASSERT_EQ(result, entry);
}

}  // namespace filesystem
}  // namespace kodama
