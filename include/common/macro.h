// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_COMMON_MACRO_H_
#define INCLUDE_COMMON_MACRO_H_

#define SIGNAL_CONNECTOR(sig)  void sig(signal_t::slot_type callback) { sig##_.connect(callback); }

#endif  // INCLUDE_COMMON_MACRO_H_
