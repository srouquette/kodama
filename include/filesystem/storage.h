// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_STORAGE_H_
#define INCLUDE_FILESYSTEM_STORAGE_H_

#include "common/macro.h"
#include "filesystem/forward_decl.h"
#include "filesystem/namespace.h"
#include "platform/pragma.h"
#include "thread/lockable.h"

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
    Storage(const Storage&)             = delete;
    Storage(Storage&&)                  = default;
    Storage& operator=(const Storage&)  = delete;
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
    virtual std::vector<entry_ptr_t> ls(const fs::path& path);
    virtual fs::file_status status(const fs::path& path) const;

    fs::path split(const std::string& url) const;

 private:
    using entry_map_t   = std::map<fs::path, entry_ptr_t>;
    using entries_t     = thread::Lockable<entry_map_t, std::mutex>;
    using lazy_status_t = std::function<fs::file_status (const fs::path& path)>;

    entry_ptr_t get_or_insert(const fs::path& path, lazy_status_t get_status);

    signal_t            on_create_;
    signal_t            on_delete_;

    mutable entries_t   entries_;
    const std::string   scheme_;
};

}  // namespace filesystem
}  // namespace kodama

WARNING_POP

#endif  // INCLUDE_FILESYSTEM_STORAGE_H_
