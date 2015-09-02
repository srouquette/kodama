// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "filesystem/property.h"
#include "filesystem/storage.h"


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

Entry::Entry(const storage_ptr_t& storage,
             const std::string& url,
             const key&)
    : mutex_{}
    , shared_mutex_{}
    , storage_{ storage }
    , url_{ url }
{}

Entry::~Entry()
{}

const std::string& Entry::url() const noexcept {
    return url_;
}

bool Entry::is_dir() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    auto storage = storage_.lock();
    if (!storage) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_device);
    }
    return storage->is_dir(*this);
}

bool Entry::exists() const {
    if (storage_.expired()) {
        return false;
    }
    std::lock_guard<std::mutex> lock{ mutex_ };
    auto storage = storage_.lock();
    return storage ? storage->exists(*this) : false;
}

void Entry::invalidate() noexcept {
    storage_.reset();
}

template<typename T>
T Entry::lock() const {
    T lock{ shared_mutex_ };
    if (!exists()) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_file_or_directory);
    }
    return lock;
}

void Entry::set_property(property_ptr_t&& property) {
    properties_.emplace(std::type_index{ typeid(*property) }, std::move(property));
}

}  // namespace filesystem
}  // namespace kodama
