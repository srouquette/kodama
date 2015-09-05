// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/url_resolver.h"
#include "filesystem/exception.h"
#include "filesystem/storage.h"

#include <iostream>

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

void UrlResolver::add(const storage_ptr_t& storage) {
    std::lock_guard<std::mutex> lock{ mutex_ };
    storages_.insert(storage->make_pair());
}

entry_ptr_t UrlResolver::resolve(const std::string& url) const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    entry_ptr_t entry;
    // try to resolve first based on the scheme
    for (const auto& storage : storages_) {
        if (url.find_first_of(storage.first) == 0) {
            if (entry = storage.second->resolve(url)) {
                return entry;
            }
            break;
        }
    }
    for (const auto& storage : storages_) {
        if (entry = storage.second->resolve(url)) {
            return entry;
        }
    }
    throw EXCEPTION(__FUNCTION__, url, no_such_file_or_directory);
}

}  // namespace filesystem
}  // namespace kodama
