// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "filesystem/storage.h"


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

Entry::Entry(const storage_ptr_t& storage,
             const std::string& url,
             const fs::file_status& status,
             const key&)
    : content_{}
    , mutex_{}
    , shared_mutex_{}
    , status_{ status }
    , storage_{ storage }
    , url_{ url }
{}

Entry::~Entry()
{}

// return a copy, no need to lock
Entry::content_t Entry::content() const {
    return content_;
}

bool Entry::exists() const {
    if (storage_.expired()) {
        return false;
    }
    std::lock_guard<std::mutex> lock{ mutex_ };
    auto storage = storage_.lock();
    return storage ? storage->exists(*this) : false;
}

bool Entry::is_dir() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    auto storage = lock_storage();
    return storage->is_dir(*this);
}

void Entry::invalidate() noexcept {
    storage_.reset();
}

void Entry::ls() {
    auto storage = lock_storage();
    auto content = storage->ls(*this);
    std::lock_guard<std::mutex> lock{ mutex_ };
    std::swap(content_, content);
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

const std::string& Entry::url() const noexcept {
    return url_;
}

storage_ptr_t Entry::lock_storage() const {
    auto storage = storage_.lock();
    if (!storage) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_device);
    }
    return storage;
}

void Entry::throws_if_nonexistent() const {
    if (!exists()) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_file_or_directory);
    }
}

}  // namespace filesystem
}  // namespace kodama
