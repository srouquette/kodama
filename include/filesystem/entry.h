// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_ENTRY_H_
#define INCLUDE_FILESYSTEM_ENTRY_H_

#include "common/macro.h"
#include "filesystem/forward_decl.h"
#include "filesystem/namespace.h"
#include "thread/lockable.h"

#include <boost/signals2.hpp>

#include <string>
#include <vector>


namespace kodama { namespace filesystem {

class Entry {
 public:
    using entries_t = std::vector<entry_ptr_t>;
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

    Entry(const Entry&)         = delete;
    Entry(Entry&&)              = default;
    Entry& operator=(const Entry&) = delete;
    Entry& operator=(Entry&&)   = default;
    ~Entry();

    bool operator==(const Entry& rhs) const noexcept;

    SIGNAL_CONNECTOR(on_update);

    entries_t content()         const noexcept;
    bool exists()               const noexcept;
    bool is_dir()               const;
    const fs::path& path()      const noexcept;
    const std::string& url()    const noexcept;
    void invalidate()           noexcept;
    void ls();

 private:
    using content_t = thread::Lockable<entries_t, std::mutex>;
    using status_t  = thread::Lockable<fs::file_status, std::mutex>;

    storage_ptr_t get_storage() const;
    bool is_dir(const Storage& storage);

    signal_t                    on_update_;

    // weak_ptr to prevent circular ref
    std::weak_ptr<Storage>      storage_;
    const std::string           url_;
    const fs::path              path_;
    mutable status_t            status_;
    mutable content_t           content_;
};

}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_ENTRY_H_
