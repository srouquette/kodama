// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/pattern/storage_helper.h"
#include "filesystem/storage.h"


namespace kodama { namespace filesystem {

StorageHelper::~StorageHelper()
{}

storage_ptr_t StorageHelper::storage() const {
    return std::make_shared<Storage>("file://");
}

fs::path StorageHelper::temp_directory_path() const {
    return fs::temp_directory_path();
}

}  // namespace filesystem
}  // namespace kodama
