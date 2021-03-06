// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_URL_RESOLVER_H_
#define INCLUDE_FILESYSTEM_URL_RESOLVER_H_

#include "common/macro.h"
#include "filesystem/forward_decl.h"

#include <boost/signals2.hpp>

#include <map>
#include <string>
#include <mutex>  // NOLINT


namespace kodama { namespace filesystem {

class UrlResolver {
 public:
    using signal_t = boost::signals2::signal<void (const Storage&)>;

    void add(const storage_ptr_t& storage);
    entry_ptr_t resolve(const std::string& url) const;

    SIGNAL_CONNECTOR(on_add);

 private:
    std::map<std::string, storage_ptr_t>  storages_;
    mutable std::mutex mutex_;

    signal_t on_add_;
};

}  // namespace filesystem
}  // namespace kodama


#endif  // INCLUDE_FILESYSTEM_URL_RESOLVER_H_
