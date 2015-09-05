// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_ENTRY_H_
#define INCLUDE_FILESYSTEM_ENTRY_H_

#include "common/macro.h"
#include "filesystem/forward_decl.h"
#include "filesystem/namespace.h"
#include "thread/lock.h"

#include <boost/signals2.hpp>

#include <mutex>  // NOLINT
#include <string>
#include <vector>


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

class Entry {
 public:
    using content_t = std::vector<entry_ptr_t>;
    using signal_t  = boost::signals2::signal<void (const Entry&)>;

    friend class Storage;
    class key {
        friend class Storage;
        key() {}
    };

    Entry(const storage_ptr_t& storage,
          const std::string& url,
          const fs::path& path,
          const fs::file_status& status,
          const key&);

    Entry(const Entry&)             = default;
    Entry(Entry&&)                  = default;
    Entry& operator=(const Entry&)  = default;
    Entry& operator=(Entry&&)       = default;
    ~Entry();

    SIGNAL_CONNECTOR(on_update);

    content_t content() const;
    bool exists() const;
    bool is_dir() const;
    void invalidate() noexcept;
    void ls();
    const fs::path& path() const;
    thread::shared_lock_t shared_lock() const;
    thread::unique_lock_t unique_lock() const;
    const std::string& url() const noexcept;

 private:
    void throws_if_nonexistent() const;
    void throws_if_storage_null() const;
    void safe_update_status() const;
    void update_status() const;

    signal_t                        on_update_;

    content_t                       content_;
    mutable std::mutex              mutex_;
    const fs::path                  path_;
    mutable boost::shared_mutex     shared_mutex_;
    mutable fs::file_status         status_;
    storage_ptr_t                   storage_;
    const std::string               url_;
};

}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_ENTRY_H_
