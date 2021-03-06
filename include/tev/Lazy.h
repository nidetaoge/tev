// This file was developed by Thomas Müller <thomas94@gmx.net>.
// It is published under the BSD 3-Clause License within the LICENSE file.

#pragma once

#include <tev/Common.h>
#include <tev/ThreadPool.h>

#include <chrono>
#include <functional>
#include <future>

TEV_NAMESPACE_BEGIN

// Encapsulates a lazy, potentially asynchronous computation
// of some value. The public interface of this object is not
// thread-safe, i.e. it is expected to never be used from
// multiple threads at once.
template <typename T>
class Lazy {
public:
    Lazy(std::function<T(void)> compute)
    : Lazy{compute, nullptr} {
    }

    Lazy(std::function<T(void)> compute, ThreadPool* threadPool)
    : mThreadPool{threadPool}, mCompute{compute} {
    }

    T get() {
        if (mIsComputed) {
            return mValue;
        }

        if (mAsyncValue.valid()) {
            mValue = mAsyncValue.get();
        } else {
            mValue = mCompute();
        }

        mIsComputed = true;
        return mValue;
    }

    bool isReady() const {
        if (mIsComputed) {
            TEV_ASSERT(
                !mAsyncValue.valid(),
                "There should never be a background computation while the result is already available."
            );

            return true;
        }

        if (!mAsyncValue.valid()) {
            return false;
        }

        return mAsyncValue.wait_for(std::chrono::seconds{0}) == std::future_status::ready;
    }

    void computeAsync() {
        // No need to perform an async computation if we
        // already computed the value before or if one is
        // already running.
        if (mAsyncValue.valid() || mIsComputed) {
            return;
        }

        if (mThreadPool) {
            mAsyncValue = mThreadPool->enqueueTask(mCompute, true);
        } else {
            mAsyncValue = std::async(std::launch::async, mCompute);
        }
    }

private:
    // If this thread pool is present, use it to run tasks
    // instead of std::async.
    ThreadPool* mThreadPool = nullptr;

    std::function<T(void)> mCompute;
    std::future<T> mAsyncValue;
    T mValue;
    bool mIsComputed = false;
};

TEV_NAMESPACE_END
