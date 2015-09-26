// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test/common/gtest.h"


#if STL_FILESYSTEM_SUPPORTED

namespace std { namespace experimental { namespace filesystem { namespace v1 {
    void PrintTo(const path& p, std::ostream* os) {
        *os << p;
    }
}  // namespace v1
}  // namespace filesystem
}  // namespace experimental
}  // namespace std

#else  // STL_FILESYSTEM_SUPPORTED

namespace boost { namespace filesystem {
    void PrintTo(const path& p, std::ostream* os) {
        *os << p;
    }
}  // namespace filesystem
}  // namespace boost

#endif  // STL_FILESYSTEM_SUPPORTED
