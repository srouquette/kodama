// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_THREAD_LOCK_H_
#define INCLUDE_THREAD_LOCK_H_

#include <boost/thread/locks.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread/shared_mutex.hpp>


namespace kodama { namespace thread {

using shared_lock_t     = boost::shared_lock<boost::shared_mutex>;
using unique_lock_t     = boost::unique_lock<boost::shared_mutex>;
using upgrade_lock_t    = boost::upgrade_lock<boost::shared_mutex>;
using u2u_lock_t        = boost::upgrade_to_unique_lock<boost::shared_mutex>;

}  // namespace thread
}  // namespace kodama

#endif  // INCLUDE_THREAD_LOCK_H_
