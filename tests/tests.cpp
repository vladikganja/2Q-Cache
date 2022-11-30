#include "gtest/gtest.h"
#include "2Qcache.hpp"
#include <algorithm>

int slow_get_page(int n) {
    return n;
}

void print(const std::vector<int>& vals) {
    for (auto el : vals) {
        std::cout << el << ' ';
    }
    std::cout << std::endl;
}

TEST(test, test1) {

    constexpr size_t CACHE_SIZE = 3;
    constexpr size_t VALS_COUNT = 1000;

    cache::TwoQueueCache<int> c(CACHE_SIZE);

    std::vector<int> vals;
    std::vector<float> distribution = 
    { 33.f, 20.f, 11.f, 8.f, 4.f, 3.f, 3.f, 3.f, 3.f, 2.f, 2.f, 2.f, 1.f, 1.f, 1.f, 1.f, 
        0.6f, 0.4f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f }; // sum = 100%

    int num = 1;
    for (auto el : distribution) {
        for (int i = 0; i < (el / 100.f) * VALS_COUNT; i++) {
            vals.push_back(num);
        }
        num++;
    }

    std::random_shuffle(vals.begin(), vals.end());
    // print(vals);

    size_t hits = 0;

    for (auto el : vals) {
        // std::cout << '\n' << el;
        if (c.lookup_update(el, slow_get_page)) {
            // std::cout << " hit\n";
            hits++;
        }
        else {
            // std::cout << " miss\n";
        }
        // c.debug_print();
    }

    std::cout << hits << std::endl;

    ASSERT_GT(hits, VALS_COUNT * 0.7f);
}
