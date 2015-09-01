// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/filesystem/pattern/storage_pattern.h"


namespace kodama { namespace filesystem {

INSTANTIATE_TEST_CASE_P(StorageTest, StoragePattern,
    testing::Values(StorageHelper{}));

}  // namespace filesystem
}  // namespace kodama
