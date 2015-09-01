// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/exception.h"


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

#if STL_FILESYSTEM_ENABLED

struct filesystem_error::impl {
    explicit impl(const FILESYSTEM_NAMESPACE::path& path1)
        : what_{}
        , path1_{ path1 }
    {}

    std::string what_;
    FILESYSTEM_NAMESPACE::path path1_;
};

filesystem_error::filesystem_error(const std::string& what,
                                   const FILESYSTEM_NAMESPACE::path& path1,
                                   std::error_code error_code)
    : std::system_error(error_code, what)
{
    try {
        std::make_shared<filesystem_error::impl>(path1).swap(impl_);
    } catch (...) {}
}

const char* filesystem_error::what() const noexcept {
    if (!impl_.get()) {
        return std::system_error::what();
    }
    try {
        if (impl_->what_.empty()) {
            impl_->what_ = std::system_error::what();
            if (!impl_->path1_.empty()) {
                impl_->what_ += impl_->path1_.string();
            }
        }
        return impl_->what_.c_str();
    } catch (...) {
        return std::system_error::what();
    }
}

#endif

}  // namespace filesystem
}  // namespace kodama
