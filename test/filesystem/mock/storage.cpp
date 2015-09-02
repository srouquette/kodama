// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/mock/storage.h"
#include "filesystem/entry.h"
#include "filesystem/property_status.h"
#include "filesystem/storage.h"


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

MockStorage::MockStorage(const std::string& scheme)
    : Storage(scheme)
{}

entry_ptr_t MockStorage::make(const std::string& url) {
    auto entry = Storage::make(url);
    entry->set_property(std::make_unique<PropertyStatus>(fs::file_status{}));
    return entry;
}

}  // namespace filesystem
}  // namespace kodama
