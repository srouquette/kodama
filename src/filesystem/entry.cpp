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
    , path_{ path }
    , storage_{ storage }
    , url_{ url }
    , content_{}
    , status_{ status }
{}

Entry::~Entry()
{}

bool Entry::operator==(const Entry& rhs) const {
    return url_ == rhs.url();
}

Entry::entries_t Entry::content() const noexcept {
    return content_.clone();
}

const fs::path& Entry::path() const noexcept {
    return path_;
}

const std::string& Entry::url() const noexcept {
    return url_;
}

bool Entry::exists() const noexcept {
    try {
        std::lock_guard<status_t> lock{ status_ };
        return get_storage()->exists(status_);
    } catch (const filesystem_error&) {
        return false;
    }
}

bool Entry::is_dir() const {
    std::lock_guard<status_t> lock{ status_ };
    return get_storage()->is_dir(status_);
}

void Entry::ls() {
    std::lock_guard<status_t> lock{ status_ };
    auto storage = get_storage();
    update_status(*storage);
    if (!storage->is_dir(status_)) {
        throw EXCEPTION(__FUNCTION__, url_, not_a_directory);
    }
    content_ = storage->ls(*this);
    on_update_(*this);
}

void Entry::invalidate() noexcept {
    storage_.reset();
}

storage_ptr_t Entry::get_storage() const {
    if (storage_.expired()) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_device);
    }
    return storage_.lock();
}

void Entry::update_status(const Storage& storage) const {
    status_.assign(storage.status(*this));
    if (!storage.exists(status_)) {
        throw EXCEPTION(__FUNCTION__, url_, no_such_file_or_directory);
    }
}

}  // namespace filesystem
}  // namespace kodama
