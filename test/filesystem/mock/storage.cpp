// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/mock/storage.h"
#include "filesystem/entry.h"
#include "filesystem/storage.h"


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

MockStorage::MockStorage(const std::string& scheme)
    : Storage(scheme)
{
    ON_CALL(*this, status(testing::An<const Entry&>()))
        .WillByDefault(testing::Return(fs::file_status{}));
}

}  // namespace filesystem
}  // namespace kodama
