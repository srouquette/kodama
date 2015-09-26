// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_NAMESPACE_H_
#define INCLUDE_FILESYSTEM_NAMESPACE_H_

#if STL_FILESYSTEM_SUPPORTED

#include <filesystem>

namespace kodama { namespace filesystem {

namespace fs = std::tr2::sys;

}  // namespace filesystem
}  // namespace kodama

#else  // STL_FILESYSTEM_SUPPORTED

#include <boost/filesystem.hpp>

namespace kodama { namespace filesystem {

namespace fs = boost::filesystem;

}  // namespace filesystem
}  // namespace kodama

#endif  // STL_FILESYSTEM_SUPPORTED

#endif  // INCLUDE_FILESYSTEM_NAMESPACE_H_
