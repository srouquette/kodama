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

    void lock()     const { mutex_.lock(); }
    void unlock()   const { mutex_.unlock(); }
    bool try_lock() const { return mutex_.try_lock(); }

    Lockable& operator=(T&& value) {
        std::lock_guard<Mutex> lock{ mutex_ };
        value_ = std::forward<T>(value);
        return *this;
    }

    operator T() const {
        std::lock_guard<Mutex> lock{ mutex_ };
        return value_;
    }

    template<typename U>
    void set(T&& value, const std::lock_guard<U>&) {
        value_ = std::forward<T>(value);
    }

    template<typename U>
    const T& get(const std::lock_guard<U>&) const {
        return value_;
    }

    template<typename U>
    T& get(const std::lock_guard<U>&) {
        return value_;
    }

 private:
    mutable Mutex   mutex_;
    T               value_;
};

}  // namespace thread
}  // namespace kodama

#endif  // INCLUDE_THREAD_LOCKABLE_H_
