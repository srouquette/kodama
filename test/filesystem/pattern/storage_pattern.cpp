// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/pattern/storage_pattern.h"
#include "filesystem/storage.h"
#include "filesystem/entry.h"

namespace kodama { namespace filesystem {

const std::string DIRNAME{ "kodama_test_dir" };
const std::string FILENAME{ "kodama_test_file" };

TEST_P(StoragePattern, resolve_without_scheme) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    ASSERT_NE(nullptr, storage->resolve(path));
}

TEST_P(StoragePattern, resolve_valid_path) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    ASSERT_NE(nullptr, storage->resolve(storage->scheme() + path));
}

TEST_P(StoragePattern, resolve_valid_path_then_check_entry_url) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->scheme() + path;
    auto entry   = storage->resolve(url);
    ASSERT_NE(entry, nullptr);
    ASSERT_EQ(entry->url(), url);
}

TEST_P(StoragePattern, use_cache) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->scheme() + path;
    ASSERT_EQ(url, storage->resolve(url)->url());
    ASSERT_EQ(url, storage->resolve(url)->url());
}

TEST_P(StoragePattern, check_is_dir) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto entry   = storage->resolve(storage->scheme() + path);
    ASSERT_NE(entry, nullptr);
    ASSERT_TRUE(entry->is_dir());
}

TEST_P(StoragePattern, check_is_file) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_file(FILENAME);
    auto entry   = storage->resolve(storage->scheme() + path);
    ASSERT_NE(entry, nullptr);
    ASSERT_FALSE(entry->is_dir());
}

TEST_P(StoragePattern, resolve_invalid_path) {
    auto storage = GetParam().storage();
    ASSERT_EQ(nullptr, storage->resolve("/invalid_path"));
}

}  // namespace filesystem
}  // namespace kodama
