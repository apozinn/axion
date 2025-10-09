#include "randoms/randoms.hpp"
#include <random>

u32 randomInt(int min, int max) {
    std::random_device os_seed;
    const u32 seed = os_seed();

    std::mt19937 generator(seed);
    std::uniform_int_distribution<u32> distribute(min, max);
    return distribute(generator);
}

std::string randomString(size_t length) {
    static const std::string allowed_chars {
        "123456789ABCDFGHJKLMNPQRSTVWXZabcdfghjklmnpqrstvwxz"
    };
    static thread_local std::default_random_engine randomEngine(std::random_device{}());
    static thread_local std::uniform_int_distribution<int> randomDistribution(0, allowed_chars.size() - 1);

    std::string id(length ? length : 32, '\0');

    for (std::string::value_type& c : id) {
        c = allowed_chars[randomDistribution(randomEngine)];
    }
    return id;
}
