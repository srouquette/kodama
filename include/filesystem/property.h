// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_FILESYSTEM_PROPERTY_H_
#define INCLUDE_FILESYSTEM_PROPERTY_H_


namespace kodama { namespace filesystem {

class Property {
 public:
    virtual ~Property() = 0;
};

inline Property::~Property()
{}

}  // namespace filesystem
}  // namespace kodama

#endif  // INCLUDE_FILESYSTEM_PROPERTY_H_
