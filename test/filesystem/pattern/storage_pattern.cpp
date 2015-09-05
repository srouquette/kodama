// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/pattern/storage_pattern.h"
#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "filesystem/storage.h"

namespace kodama { namespace filesystem {

const std::string DIRNAME{ "kodama_test_dir" };
const std::string FILENAME{ "kodama_test_file" };

TEST_P(StoragePattern, resolve_without_scheme) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->to_url(path);
    if (GetParam().can_work_without_scheme()) {
        ASSERT_NE(nullptr, storage->resolve(path.string()));
    } else {
        ASSERT_EQ(nullptr, storage->resolve(url));
    }
}

TEST_P(StoragePattern, resolve_valid_path) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->to_url(path);
    ASSERT_NE(nullptr, storage->resolve(url));
}

TEST_P(StoragePattern, resolve_valid_path_then_check_entry) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->to_url(path);
    auto entry   = storage->resolve(url);
    ASSERT_NE(entry, nullptr);
    ASSERT_EQ(entry->url(), url);
    ASSERT_TRUE(entry->exists());
}

TEST_P(StoragePattern, on_create_signal) {
    auto storage  = GetParam().storage();
    auto path     = GetParam().create_dir(DIRNAME);
    auto expected = storage->to_url(path);
    std::string url;
    storage->on_create([&url](const Entry& entry) { url = entry.url(); });
    ASSERT_NE(nullptr, storage->resolve(expected));
    ASSERT_EQ(url, expected);
}

TEST_P(StoragePattern, on_delete_signal) {
    auto storage  = GetParam().storage();
    auto path     = GetParam().create_dir(DIRNAME);
    auto expected = storage->to_url(path);
    std::string url;
    storage->on_delete([&url](const Entry& entry) { url = entry.url(); });
    // TODO(Syl): delete entry from the cache
}

TEST_P(StoragePattern, use_cache) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->to_url(path);
    ASSERT_EQ(url, storage->resolve(url)->url());
    // a second time to check the cache
    ASSERT_EQ(url, storage->resolve(url)->url());
}

TEST_P(StoragePattern, check_is_dir) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->to_url(path);
    auto entry   = storage->resolve(url);
    ASSERT_NE(entry, nullptr);
    ASSERT_TRUE(entry->is_dir());
}

TEST_P(StoragePattern, check_is_file) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_file(FILENAME);
    auto url     = storage->to_url(path);
    auto entry   = storage->resolve(url);
    ASSERT_NE(entry, nullptr);
    ASSERT_FALSE(entry->is_dir());
}

TEST_P(StoragePattern, resolve_invalid_path) {
    auto storage = GetParam().storage();
    ASSERT_EQ(nullptr, storage->resolve("/invalid_path"));
}

TEST_P(StoragePattern, ls_nonexistent) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->to_url(path);
    auto entry   = storage->resolve(url);
    ASSERT_NE(entry, nullptr);
    ASSERT_TRUE(entry->is_dir());
    // TODO(Syl): removing dir should update status
    // GetParam().remove(path);
    // ASSERT_THROW(entry->ls(), filesystem_error);
}

TEST_P(StoragePattern, ls_invalid_dir) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_file(FILENAME);
    auto url     = storage->to_url(path);
    auto entry   = storage->resolve(url);
    ASSERT_NE(entry, nullptr);
    ASSERT_FALSE(entry->is_dir());
    ASSERT_THROW(entry->ls(), filesystem_error);
}

TEST_P(StoragePattern, ls) {
    auto storage = GetParam().storage();
    auto path    = GetParam().create_dir(DIRNAME);
    auto url     = storage->to_url(path);
    auto entry   = storage->resolve(url);
    std::vector<std::string> content{
        storage->to_url(GetParam().create_file(path / "file1")),
        storage->to_url(GetParam().create_file(path / "file2"))
    };
    ASSERT_NE(entry, nullptr);
    ASSERT_TRUE(entry->is_dir());
    ASSERT_NO_THROW(entry->ls());
    ASSERT_EQ(entry->content().size(), content.size());
    ASSERT_TRUE(std::equal(content.begin(), content.end(), entry->content().begin(),
                [](const std::string& lhs, const entry_ptr_t& rhs) {
                    return lhs == rhs->url();
                }));
}

}  // namespace filesystem
}  // namespace kodama
