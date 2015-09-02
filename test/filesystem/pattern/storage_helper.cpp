// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/pattern/storage_helper.h"
#include "filesystem/storage.h"

#include <fstream>  // NOLINT


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

StorageHelper::~StorageHelper() noexcept {
    for (const auto& path : entries_) {
        try {
            fs::remove(path);
        } catch (...) {}
    }
}

storage_ptr_t StorageHelper::storage() const {
    return std::make_shared<Storage>("file://");
}

bool StorageHelper::can_work_without_scheme() const {
    return true;
}

std::string StorageHelper::create_dir(std::string path) const {
    if (fs::exists(path)) {
        return path;
    }
    fs::create_directory(path);
    entries_.insert(path);
    return path;
}

std::string StorageHelper::create_file(std::string path) const {
    if (fs::exists(path)) {
        return path;
    }
    std::ofstream{ path };
    entries_.insert(path);
    return path;
}

}  // namespace filesystem
}  // namespace kodama
