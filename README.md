# RateLimiter
run time 令牌桶限速器

## Usage
```
::garden::RateLimiter limiter(10, 5); // (令牌桶总量, 补充令牌速度（每秒）)

limiter.acquire(); // 请求消耗一个令牌，返回成功/失败

limiter.wait_until_acquire(); // 请求消耗一个令牌。等待，直到请求成功。
```
