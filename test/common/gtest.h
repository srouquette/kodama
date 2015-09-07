// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_COMMON_GTEST_H_
#define TEST_COMMON_GTEST_H_

#include "filesystem/namespace.h"


#if STL_FILESYSTEM_ENABLED

namespace std { namespace experimental { namespace filesystem { namespace v1 {
    void PrintTo(const path& p, std::ostream* os);
}  // namespace v1
}  // namespace filesystem
}  // namespace experimental
}  // namespace std

#else  // STL_FILESYSTEM_ENABLED

namespace boost { namespace filesystem {
    void PrintTo(const path& p, std::ostream* os);
}  // namespace filesystem
}  // namespace boost

#endif

#endif  // TEST_COMMON_GTEST_H_
