// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_PROPERTY_STATUS_H_
#define INCLUDE_FILESYSTEM_PROPERTY_STATUS_H_

#include "filesystem/property.h"
#include "filesystem/namespace.h"


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

class PropertyStatus : public Property {
 public:
    explicit PropertyStatus(const fs::file_status& status);
    ~PropertyStatus() override;

    const fs::file_status& value() const noexcept;

 private:
    fs::file_status status_;
};

}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_PROPERTY_STATUS_H_
