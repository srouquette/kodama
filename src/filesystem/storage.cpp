// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/storage.h"
#include "filesystem/entry.h"
#include "filesystem/exception.h"

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

Storage::Storage(const std::string& scheme)
    : entries_{}
    , mutex_{}
    , scheme_{ scheme }
    , on_create_{}
    , on_delete_{}
    , on_update_{}
{}

Storage::~Storage()
{}

const std::string& Storage::scheme() const {
    return scheme_;
}

entry_ptr_t Storage::resolve(const std::string& url) {
    std::lock_guard<std::mutex> lock{ mutex_ };
    auto path = split(url);
    auto str = path.string();
    auto lb = entries_.lower_bound(str);
    if (lb != entries_.end() && !entries_.key_comp()(str, lb->first)) {
        return lb->second;
    }
    auto status = fs::status(path);
    if (!fs::exists(status)) {
        return nullptr;
    }
    auto entry = create(url, status);
    entries_.insert(lb, entries_t::value_type{ str, entry });
    on_create_(*entry);
    return entry;
}

entry_ptr_t Storage::create(const std::string& url, const fs::file_status& status) {
    return std::make_shared<Entry>(shared_from_this(), url, status, Entry::key{});
}

bool Storage::is_dir(const Entry& entry) const {
    return fs::is_directory(entry.status_);
}

bool Storage::exists(const Entry& entry) const {
    return fs::exists(entry.status_);
}

fs::path Storage::split(const std::string& url) const {
    if (url.find_first_of(scheme_) == 0) {
        return url.substr(scheme_.size());
    } else {
        return url;
    }
}

}  // namespace filesystem
}  // namespace kodama
