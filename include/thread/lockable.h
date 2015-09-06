// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef INCLUDE_THREAD_LOCKABLE_H_
#define INCLUDE_THREAD_LOCKABLE_H_

#include <mutex>  // NOLINT


namespace kodama { namespace thread {

template<typename T, typename Mutex>
class Lockable {
 public:
    explicit Lockable(T&& value)
        : mutex_{}
        , value_{ std::forward<T>(value) }
    {}
    explicit Lockable(const T& value)
        : mutex_{}
        , value_{ value }
    {}

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

    Lockable& operator=(T&& value) {
        std::lock_guard<Mutex> lock{ mutex_ };
        value_ = std::forward<T>(value);
        return *this;
    }
    const T clone() const {
        std::lock_guard<Mutex> lock{ mutex_ };
        return value_;
    }

    // unsafe, need to be locked externally
    void assign(T&& value) {
        value_ = std::forward<T>(value);
    }
    // unsafe, need to be locked externally
    operator const T&() const {
        return value_;
    }

 private:
    mutable Mutex   mutex_;
    T               value_;
};

}  // namespace thread
}  // namespace kodama

#endif  // INCLUDE_THREAD_LOCKABLE_H_
