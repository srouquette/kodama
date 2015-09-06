// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/storage.h"
#include "filesystem/entry.h"
#include "filesystem/exception.h"

#define USE_DIR_RANGE_ITERATOR       (MSC_VER_ >= 1900 || BOOST_VERSION >= 105900)

#if !USE_DIR_RANGE_ITERATOR
#include <boost/range/iterator_range_core.hpp>
#endif

namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

Storage::Storage(const std::string& scheme)
    : on_create_{}
    , on_delete_{}
    , entries_{}
    , mutex_{}
    , scheme_{ scheme }
{}

Storage::~Storage()
{}

std::pair<std::string, storage_ptr_t> Storage::make_pair() {
    return std::make_pair(scheme_, shared_from_this());
}

entry_ptr_t Storage::resolve(const std::string& url) {
    std::lock_guard<std::mutex> lock{ mutex_ };
    return get_or_create(split(url), [](const fs::path& path) { return fs::status(path); });
}

std::string Storage::to_url(const fs::path& path) const {
    return scheme_ + path.string();
}

entry_ptr_t Storage::get_or_create(const fs::path& path, lazy_status_t get_status) {
    auto str = path.string();
    auto lb = entries_.lower_bound(str);
    if (lb != entries_.end() && !entries_.key_comp()(str, lb->first)) {
        return lb->second;
    }
    auto status = get_status(path);
    if (!fs::exists(status)) {
        return nullptr;
    }
    auto entry = create(path, status);
    entries_.insert(lb, entries_t::value_type{ str, entry });
    return entry;
}

entry_ptr_t Storage::create(const fs::path& path, const fs::file_status& status) {
    auto entry = std::make_shared<Entry>(shared_from_this(), to_url(path), path, status, Entry::key{});
    on_create_(*entry);
    return entry;
}

bool Storage::exists(const Entry& entry) const {
    return fs::exists(entry.status());
}

bool Storage::is_dir(const Entry& entry) const {
    return fs::is_directory(entry.status());
}

std::vector<entry_ptr_t> Storage::ls(const Entry& entry) {
    if (!exists(entry)) {
        throw EXCEPTION(__FUNCTION__, entry.url(), no_such_file_or_directory);
    }
    if (!is_dir(entry)) {
        throw EXCEPTION(__FUNCTION__, entry.url(), not_a_directory);
    }
    auto path = entry.path();
    std::lock_guard<std::mutex> lock{ mutex_ };
    std::vector<entry_ptr_t> content;
#if USE_DIR_RANGE_ITERATOR
    for (const auto& dir_entry : fs::directory_iterator(path)) {
#else
    for (const auto& dir_entry : boost::make_iterator_range(fs::directory_iterator(path), fs::directory_iterator())) {
#endif
        content.push_back(
            get_or_create(dir_entry.path(),
                          [&dir_entry](const fs::path&) {
                            return dir_entry.status();
                          }));
    }
    return content;
}

fs::file_status Storage::status(const Entry& entry) const {
    return fs::status(split(entry.url()));
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
