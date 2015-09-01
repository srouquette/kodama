// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_ENTRY_H_
#define INCLUDE_FILESYSTEM_ENTRY_H_

#include "filesystem/forward_decl.h"
#include "filesystem/namespace.h"
#include "thread/lock.h"

#include <string>


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

class Entry {
 public:
    friend Storage;
    class key {
        friend class Storage;
        key() {}
    };

    Entry(const storage_ptr_t& storage,
          const std::string& url,
          const fs::file_status& status,
          const key&);

    Entry(const Entry&)             = default;
    Entry(Entry&&)                  = default;
    Entry& operator=(const Entry&)  = default;
    Entry& operator=(Entry&&)       = default;
    ~Entry();

    const std::string& url() const noexcept;
    bool is_dir() const;
    bool exists() const;
    void invalidate() noexcept;

    template<typename T>
    T lock() const;

 private:
    mutable boost::shared_mutex     mutex_;
    fs::file_status                 status_;
    std::weak_ptr<Storage>          storage_;
    const std::string               url_;
};

}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_ENTRY_H_
