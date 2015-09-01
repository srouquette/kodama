// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_NAMESPACE_H_
#define INCLUDE_FILESYSTEM_NAMESPACE_H_

#if STL_FILESYSTEM_ENABLED
    #include <filesystem>
    #define FILESYSTEM_NAMESPACE    std::tr2::sys
#else
    #include <boost/filesystem.hpp>
    #define FILESYSTEM_NAMESPACE    boost::filesystem
#endif

#endif  // INCLUDE_FILESYSTEM_NAMESPACE_H_
