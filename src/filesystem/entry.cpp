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

void Entry::throws_if_nonexistent() const {
    if (!exists()) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_file_or_directory);
    }
}

thread::shared_lock_t Entry::shared_lock() const {
    thread::shared_lock_t lock{ shared_mutex_ };
    throws_if_nonexistent();
    return lock;
}

thread::unique_lock_t Entry::unique_lock() const {
    thread::unique_lock_t lock{ shared_mutex_ };
    throws_if_nonexistent();
    return lock;
}

void Entry::invalidate() noexcept {
    storage_.reset();
}

void Entry::set_property(property_ptr_t&& property) {
#if _MSC_VER >= 1900
    properties_.insert_or_assign(std::type_index{ typeid(*property) }, std::move(property));
#else
    properties_[std::type_index{ typeid(*property) }] = std::move(property);
#endif
}

}  // namespace filesystem
}  // namespace kodama
