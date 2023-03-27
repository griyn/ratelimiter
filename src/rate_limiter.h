#pragma once

#include <chrono>
#include <algorithm>
#include <mutex>
#include <thread>

namespace garden {

// 令牌桶限速
class RateLimiter {
public:
    explicit RateLimiter(uint32_t cap, uint32_t rate) :
            _last_time_s(get_now_s()),
            _capacity(cap),
            _rate(rate),
            _tokens(_rate) {} // 默认放置rate个令牌

    static uint64_t get_now_s() {
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    bool acquire() {
        std::lock_guard<std::mutex> guard(_lock);
        auto now_s = get_now_s();
        uint64_t interval_s = now_s - _last_time_s;
        // 该段时间内预期放置的令牌数
        uint32_t max_tokens = _tokens + interval_s * _rate;
        max_tokens = std::max(max_tokens, _tokens); // 避免_tokens溢出
        _tokens = std::min(_capacity, max_tokens);
        _last_time_s = now_s;
        if (_tokens < 1) { 
            return false;
        }
        _tokens -= 1;
        return true;
    }

    // sleep 直到 acquire 成功
    void wait_until_acquire(int wait_s = 1) {
        while (acquire() == false) {
            std::this_thread::sleep_for(std::chrono::seconds(wait_s));
        }
    }

private:
    std::mutex _lock; // 避免线程切换，导致_time_stamp_s 出现时间回退或跳跃
    uint64_t _last_time_s;
    const uint32_t _capacity; // 桶的容量
    const uint32_t _rate;     // 每秒令牌放入速度
    uint32_t _tokens; // 当前令牌数量
};

} // garden
