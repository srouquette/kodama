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

    const std::string& scheme() const;
    virtual entry_ptr_t resolve(const std::string& url);

    SIGNAL_CONNECTOR(on_create);
    SIGNAL_CONNECTOR(on_delete);
    SIGNAL_CONNECTOR(on_update);

 protected:
    virtual entry_ptr_t create(const std::string& url, const fs::file_status& status);
    virtual bool is_dir(const Entry& entry) const;
    virtual bool exists(const Entry& entry) const;
    fs::path split(const std::string& url) const;

 private:
    using entries_t = std::map<std::string, entry_ptr_t>;

    entries_t           entries_;
    mutable std::mutex  mutex_;
    std::string         scheme_;

    signal_t            on_create_;
    signal_t            on_delete_;
    signal_t            on_update_;
};

}  // namespace filesystem
}  // namespace kodama

WARNING_POP

#endif  // INCLUDE_FILESYSTEM_STORAGE_H_
