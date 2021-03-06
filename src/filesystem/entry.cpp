// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/entry.h"
#include "filesystem/exception.h"
#include "filesystem/storage.h"

#include <boost/signals2.hpp>


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

Entry::Entry(const storage_ptr_t& storage,
             const std::string& url,
             const fs::path& path,
             const fs::file_status& status,
             const key&)
    : on_update_{}
    , content_{}
    , mutex_{}
    , path_{ path }
    , shared_mutex_{}
    , status_{ status }
    , storage_{ storage }
    , url_{ url }
{}

Entry::~Entry()
{}

Entry::content_t Entry::content() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    return content_;
}

const fs::path& Entry::path() const noexcept {
    return path_;
}

const std::string& Entry::url() const noexcept {
    return url_;
}

bool Entry::exists() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    try {
        return storage()->exists(*this);
    } catch (const filesystem_error&) {
        return false;
    }
}

bool Entry::is_dir() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    return storage()->is_dir(*this);
}

void Entry::invalidate() noexcept {
    storage_.reset();
}

void Entry::ls() {
    update_status_safely();
    auto content = storage()->ls(*this);
    std::lock_guard<std::mutex> lock{ mutex_ };
    std::swap(content_, content);
    on_update_(*this);
}

thread::shared_lock_t Entry::shared_lock() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    throws_if_nonexistent();
    return thread::shared_lock_t{ shared_mutex_ };
}

thread::unique_lock_t Entry::unique_lock() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    throws_if_nonexistent();
    return thread::unique_lock_t{ shared_mutex_ };
}

storage_ptr_t Entry::storage() const {
    if (storage_.expired()) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_device);
    }
    return storage_.lock();
}

void Entry::throws_if_nonexistent() const {
    update_status();
    if (!storage()->exists(*this)) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_file_or_directory);
    }
}

void Entry::update_status_safely() const {
    std::lock_guard<std::mutex> lock{ mutex_ };
    update_status();
}

void Entry::update_status() const {
    status_ = storage()->status(*this);
}

}  // namespace filesystem
}  // namespace kodama
