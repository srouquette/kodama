// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_FORWARD_DECL_H_
#define INCLUDE_FILESYSTEM_FORWARD_DECL_H_

#include <memory>


namespace kodama { namespace filesystem {
class Storage;
class Entry;

using storage_ptr_t = std::shared_ptr<Storage>;
using entry_ptr_t   = std::shared_ptr<Entry>;

}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_FORWARD_DECL_H_
