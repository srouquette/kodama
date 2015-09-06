// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_THREAD_LOCKABLE_H_
#define INCLUDE_THREAD_LOCKABLE_H_

namespace kodama { namespace thread {

template<typename T, typename Mutex>
class Lockable {
 public:
    explicit Lockable(const T& value)
        : mutex_{}
        , value_{ value }
    {}
    Lockable& operator=(T&& value) {
        value_ = std::forward<T>(value);
        return *this;
    }

    Lockable()                              = default;
    Lockable(const Lockable&)               = default;
    Lockable(Lockable&&)                    = default;
    Lockable& operator=(const Lockable&)    = default;
    Lockable& operator=(Lockable&&)         = default;
    ~Lockable()                             = default;

    void lock() {
        mutex_.lock();
    }
    bool try_lock() {
        return mutex_.try_lock();
    }
    void unlock() {
        mutex_.unlock();
    }
    const T& value() const {
        return value_;
    }

 private:
    mutable Mutex   mutex_;
    T               value_;
};

}  // namespace thread
}  // namespace kodama

#endif  // INCLUDE_THREAD_LOCKABLE_H_
