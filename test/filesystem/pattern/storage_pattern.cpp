// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/pattern/storage_pattern.h"
#include "filesystem/storage.h"
#include "filesystem/entry.h"

namespace kodama { namespace filesystem {

TEST_P(StoragePattern, resolve_valid_path) {
    auto storage = GetParam().storage();
    auto path = GetParam().temp_directory_path().string();
    ASSERT_NE(nullptr, storage->resolve(storage->scheme() + path));
}

TEST_P(StoragePattern, resolve_valid_path_entry_url) {
    auto storage = GetParam().storage();
    auto path = GetParam().temp_directory_path().string();
    std::string url = storage->scheme() + path;
    entry_ptr_t entry;
    ASSERT_NE(nullptr, entry = storage->resolve(url));
    ASSERT_EQ(url, entry->url());
}

TEST_P(StoragePattern, resolve_invalid_path) {
    auto storage = GetParam().storage();
    ASSERT_EQ(nullptr, storage->resolve("/invalid_path"));
}

}  // namespace filesystem
}  // namespace kodama
