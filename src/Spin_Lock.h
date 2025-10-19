#pragma once
#include <atomic>
#include <thread>     // for yield
#if defined(__x86_64__) || defined(_M_X64)
#   include <immintrin.h> // for _mm_pause (x86 hint)
#endif

class spin_lock {
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;

public:
    spin_lock() noexcept = default;
    spin_lock(const spin_lock&) = delete;
    spin_lock& operator=(const spin_lock&) = delete;

    void lock() noexcept {
        // 自旋直到拿到鎖
        while (flag_.test_and_set(std::memory_order_acquire)) {
            // 在 busy-wait 期間使用 CPU hint 減少資源浪費
#if defined(__x86_64__) || defined(_M_X64)
            _mm_pause();
#else
            std::this_thread::yield();
#endif
        }
    }

    bool try_lock() noexcept {
        // 嘗試立即取得鎖，若失敗不自旋
        return !flag_.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept {
        flag_.clear(std::memory_order_release);
    }
};
