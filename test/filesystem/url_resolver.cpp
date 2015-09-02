// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/url_resolver.h"
#include "filesystem/exception.h"
#include "test/filesystem/mock/storage.h"
#include "gmock/gmock.h"

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

const std::string SCHEME{ "mock://" };
const std::string PATH{ "path" };
const std::string URL{ SCHEME + PATH };

TEST(UrlResolverTest, add_storage) {
    UrlResolver resolver;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    EXPECT_NO_THROW(resolver.add(storage));
}

TEST(UrlResolverTest, add_storage_with_existing_scheme) {
    UrlResolver resolver;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    EXPECT_NO_THROW(resolver.add(storage));
    EXPECT_NO_THROW(resolver.add(storage));
}

TEST(UrlResolverTest, resolve_without_storage) {
    UrlResolver resolver;
    EXPECT_THROW(resolver.resolve(URL), filesystem_error);
}

TEST(UrlResolverTest, cannot_resolve_url) {
    UrlResolver resolver;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    EXPECT_CALL(*storage, resolve(URL)).WillOnce(testing::Return(nullptr));
    EXPECT_NO_THROW(resolver.add(storage));
    EXPECT_THROW(resolver.resolve(URL), filesystem_error);
}

TEST(UrlResolverTest, can_resolve_url) {
    UrlResolver resolver;
    entry_ptr_t result;
    auto storage = std::make_shared<MockStorage>(SCHEME);
    auto entry = storage->make(URL);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*storage, resolve(URL)).WillOnce(testing::Return(entry));
    EXPECT_NO_THROW(resolver.add(storage));
    EXPECT_NO_THROW(result = resolver.resolve(URL));
    EXPECT_EQ(result, entry);
}

TEST(UrlResolverTest, resolve_url_with_appropriate_storage) {
    UrlResolver resolver;
    entry_ptr_t result;
    const std::string good_scheme{ "good://" };
    const std::string bad_scheme{ "bad://" };
    const std::string url = good_scheme + PATH;
    auto good_storage = std::make_shared<MockStorage>(good_scheme);
    auto bad_storage  = std::make_shared<MockStorage>(bad_scheme);
    auto entry = good_storage->make(good_scheme);
    ASSERT_NE(entry, nullptr);
    EXPECT_CALL(*bad_storage, resolve(url)).WillOnce(testing::Return(nullptr));
    EXPECT_CALL(*good_storage, resolve(url)).WillOnce(testing::Return(entry));
    EXPECT_NO_THROW(resolver.add(bad_storage));
    EXPECT_NO_THROW(resolver.add(good_storage));
    EXPECT_NO_THROW(result = resolver.resolve(url));
    EXPECT_EQ(result, entry);
}

}  // namespace filesystem
}  // namespace kodama
