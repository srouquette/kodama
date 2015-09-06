// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_COMMON_EXCEPTION_H
#define TEST_COMMON_EXCEPTION_H

#include "filesystem/exception.h"


namespace kodama { namespace common {

#define ASSERT_EX_CODE(statement, expected_exception, errorcode)\
try {                                                           \
    statement;                                                  \
    FAIL() << "expected " << #expected_exception;               \
} catch (const expected_exception& e) {                         \
    ASSERT_EQ(e.code(), MAKE_ERROR_CODE(errorcode));            \
} catch (...) {                                                 \
    FAIL() << "expected " << #expected_exception;               \
}

}  // namespace common
}  // namespace kodama

#endif  // TEST_COMMON_EXCEPTION_H
