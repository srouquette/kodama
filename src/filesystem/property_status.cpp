// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "filesystem/property_status.h"


namespace kodama { namespace filesystem {
namespace fs = FILESYSTEM_NAMESPACE;

PropertyStatus::PropertyStatus(const fs::file_status& status)
    : status_{ status }
{}

PropertyStatus::~PropertyStatus()
{}

const fs::file_status& PropertyStatus::value() const noexcept {
    return status_;
}

}  // namespace filesystem
}  // namespace kodama
