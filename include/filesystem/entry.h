// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_ENTRY_H_
#define INCLUDE_FILESYSTEM_ENTRY_H_

#include "filesystem/forward_decl.h"
#include "thread/lock.h"

#include <mutex>  // NOLINT
#include <string>
#include <typeindex>
#include <unordered_map>


namespace kodama { namespace filesystem {

class Entry {
 public:
    friend Storage;
    class key {
        friend class Storage;
        key() {}
    };

    Entry(const storage_ptr_t& storage,
          const std::string& url,
          const key&);

    Entry(const Entry&)             = default;
    Entry(Entry&&)                  = default;
    Entry& operator=(const Entry&)  = default;
    Entry& operator=(Entry&&)       = default;
    ~Entry();

    const std::string& url() const noexcept;
    bool is_dir() const;
    bool exists() const;
    thread::shared_lock_t shared_lock() const;
    thread::unique_lock_t unique_lock() const;
    void throws_if_nonexistent() const;
    void invalidate() noexcept;

    void set_property(property_ptr_t&& property);

    template<typename T>
    const T& get_property() const;

 private:
    using property_map_t = std::unordered_map<std::type_index, property_ptr_t>;
    mutable std::mutex              mutex_;
    mutable boost::shared_mutex     shared_mutex_;
    std::weak_ptr<Storage>          storage_;
    const std::string               url_;
    property_map_t                  properties_;
};

// templates

template<typename T>
const T& Entry::get_property() const {
    return *dynamic_cast<T*>(properties_.at(std::type_index{ typeid(T) }).get());
}


}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_ENTRY_H_
