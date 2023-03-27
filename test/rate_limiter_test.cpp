#include <iostream>
#include "rate_limiter.h"

int main() {
    int counter = 0;
    ::garden::RateLimiter limiter(10, 5);
    while (true) {
        limiter.wait_until_acquire();
        std::cout << "acquire:" << ++counter << std::endl;
    }

    return 0;
}
