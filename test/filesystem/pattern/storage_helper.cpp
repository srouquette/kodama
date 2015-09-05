// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/pattern/storage_helper.h"
#include "filesystem/storage.h"

#include <fstream>  // NOLINT
#include <iostream>
#include <random>


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

StorageHelper::~StorageHelper() noexcept {
    for (const auto& path : entries_) {
        try {
            fs::remove_all(path);
        } catch (...) {}
    }
}

storage_ptr_t StorageHelper::storage() const {
    return std::make_shared<Storage>("file://");
}

bool StorageHelper::can_work_without_scheme() const {
    return true;
}

fs::path StorageHelper::create_dir(fs::path path) const {
    fs::create_directory(path);
    entries_.insert(path);
    return path;
}

fs::path StorageHelper::create_file(fs::path path) const {
    std::ofstream{ path.string() };
    entries_.insert(path);
    return path;
}

void StorageHelper::remove(const fs::path& path) const {
    entries_.erase(path);
    try {
        fs::remove_all(path);
    } catch (const std::exception& e) {}
}

}  // namespace filesystem
}  // namespace kodama
