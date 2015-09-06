// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_EXCEPTION_H_
#define INCLUDE_FILESYSTEM_EXCEPTION_H_

#include "filesystem/namespace.h"


namespace kodama { namespace filesystem {

#define EXCEPTION(what, path, errorcode) filesystem_error(what, path, MAKE_ERROR_CODE(errorcode))

#if STL_FILESYSTEM_ENABLED

#include <system_error>  // NOLINT

#define MAKE_ERROR_CODE(errorcode) std::make_error_code(std::errc::errorcode)

class filesystem_error : public std::system_error {
 public:
    filesystem_error(const std::string& what,
                     const FILESYSTEM_NAMESPACE::path& path1,
                     std::error_code error_code);

    const char* what() const noexcept override;

 private:
    struct impl;
    std::shared_ptr<impl>   impl_;
};

#else  // STL_FILESYSTEM_ENABLED

#define MAKE_ERROR_CODE(errorcode) std::make_error_code(boost::system::errc::errorcode)

class filesystem_error : public boost::filesystem::filesystem_error {
 public:
    filesystem_error(const std::string& what,
                     const FILESYSTEM_NAMESPACE::path& path1,
                     boost::system::error_code error_code)
        : boost::filesystem::filesystem_error(what, path1, error_code)
    {}
};

#endif  // STL_FILESYSTEM_ENABLED

}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_EXCEPTION_H_
