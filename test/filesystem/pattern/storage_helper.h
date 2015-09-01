// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_FILESYSTEM_PATTERN_STORAGE_HELPER_H_
#define TEST_FILESYSTEM_PATTERN_STORAGE_HELPER_H_

#include "filesystem/forward_decl.h"
#include "filesystem/namespace.h"

#include <boost/filesystem.hpp>


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

class StorageHelper {
 public:
    StorageHelper()                                 = default;
    StorageHelper(const StorageHelper&)             = default;
    StorageHelper(StorageHelper&&)                  = default;
    StorageHelper& operator=(const StorageHelper&)  = default;
    StorageHelper& operator=(StorageHelper&&)       = default;
    virtual ~StorageHelper();

    virtual storage_ptr_t storage() const;
    virtual fs::path temp_directory_path() const;
};

}  // namespace filesystem
}  // namespace kodama

#endif  // TEST_FILESYSTEM_PATTERN_STORAGE_HELPER_H_
