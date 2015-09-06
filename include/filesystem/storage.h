// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_STORAGE_H_
#define INCLUDE_FILESYSTEM_STORAGE_H_

#include "common/macro.h"
#include "filesystem/forward_decl.h"
#include "platform/pragma.h"

#include "filesystem/namespace.h"

#include <boost/signals2.hpp>

#include <map>
#include <mutex>  // NOLINT
#include <string>

WARNING_PUSH
WARNING_DISABLE_GCC("-Wnon-virtual-dtor")  // enable_shared_from_this


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

class Storage : public std::enable_shared_from_this<Storage> {
 public:
    friend Entry;
    using signal_t = boost::signals2::signal<void (const Entry&)>;

    explicit Storage(const std::string& scheme);
    Storage(const Storage&)             = default;
    Storage(Storage&&)                  = default;
    Storage& operator=(const Storage&)  = default;
    Storage& operator=(Storage&&)       = default;
    virtual ~Storage();

    SIGNAL_CONNECTOR(on_create);
    SIGNAL_CONNECTOR(on_delete);

    std::pair<std::string, storage_ptr_t> make_pair();
    virtual entry_ptr_t resolve(const std::string& url);
    std::string to_url(const fs::path& path) const;

 protected:
    virtual entry_ptr_t create(const fs::path& path, const fs::file_status& status);
    virtual bool exists(const fs::file_status& status) const;
    virtual bool is_dir(const fs::file_status& status) const;
    // gmock doesn't like fs::path as a parameter, I'll pass the Entry instead
    virtual std::vector<entry_ptr_t> ls(const Entry& entry);
    virtual fs::file_status status(const Entry& entry) const;

    fs::path split(const std::string& url) const;

 private:
    using entries_t = std::map<std::string, entry_ptr_t>;
    using lazy_status_t = std::function<fs::file_status (const fs::path& path)>;

    entry_ptr_t get_or_create(const fs::path& path, lazy_status_t get_status);

    signal_t            on_create_;
    signal_t            on_delete_;

    entries_t           entries_;
    mutable std::mutex  mutex_;
    std::string         scheme_;
};

}  // namespace filesystem
}  // namespace kodama

WARNING_POP

#endif  // INCLUDE_FILESYSTEM_STORAGE_H_
