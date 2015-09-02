// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/mock/storage.h"
#include "filesystem/property_status.h"
#include "filesystem/storage.h"


namespace kodama { namespace filesystem {

MockStorage::MockStorage(const std::string& scheme)
    : Storage(scheme)
{}

entry_ptr_t MockStorage::make(const std::string& url) {
    return Storage::make(url);
}

}  // namespace filesystem
}  // namespace kodama
