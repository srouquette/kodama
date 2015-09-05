// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_FILESYSTEM_MOCK_STORAGE_H
#define TEST_FILESYSTEM_MOCK_STORAGE_H

#include "filesystem/forward_decl.h"
#include "filesystem/entry.h"
#include "filesystem/storage.h"

#include "gmock/gmock.h"


namespace kodama { namespace filesystem {

class MockStorage : public Storage {
 public:
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58751
    using Storage::Storage;
    using Storage::create;

    // explicit MockStorage(const std::string& scheme) : Storage{scheme} {}

    // entry_ptr_t create(const std::string& url, const fs::file_status& status) {
    //     return Storage::create(url, status);
    // }

    MOCK_METHOD1(resolve, entry_ptr_t (const std::string& url));
    MOCK_CONST_METHOD1(is_dir, bool (const Entry& entry));
    MOCK_CONST_METHOD1(exists, bool (const Entry& entry));
    MOCK_METHOD1(ls, std::vector<entry_ptr_t> (const Entry& entry));
};

}  // namespace filesystem
}  // namespace kodama

#endif  // TEST_FILESYSTEM_MOCK_STORAGE_H
