// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_PLATFORM_PRAGMA_H_
#define INCLUDE_PLATFORM_PRAGMA_H_

#ifdef _MSC_VER
#define WARNING_PUSH                __pragma(warning(push))
#define WARNING_DISABLE_GCC(warn)
#define WARNING_DISABLE_MSVC(warn)  __pragma(warning(disable: warn))
#define WARNING_POP                 __pragma(warning(pop))
#endif

#ifdef __GNUC__
#define WARNING_PUSH                _Pragma("GCC diagnostic push")
#define STRINGIFY(x)                #x
#define WARNING_DISABLE_GCC(warn)   _Pragma(STRINGIFY(GCC diagnostic ignored warn))
#define WARNING_DISABLE_MSVC(warn)
#define WARNING_POP                 _Pragma("GCC diagnostic pop")
#endif

#endif  // INCLUDE_PLATFORM_PRAGMA_H_
