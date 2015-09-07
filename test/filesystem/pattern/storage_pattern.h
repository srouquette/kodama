// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_FILESYSTEM_PATTERN_STORAGE_PATTERN_H_
#define TEST_FILESYSTEM_PATTERN_STORAGE_PATTERN_H_

#include "test/filesystem/pattern/storage_helper.h"

#include <gtest/gtest.h>


namespace kodama { namespace filesystem {

class StoragePattern : public ::testing::TestWithParam<StorageHelper>
{};

}  // namespace filesystem
}  // namespace kodama

#endif  // TEST_FILESYSTEM_PATTERN_STORAGE_PATTERN_H_
