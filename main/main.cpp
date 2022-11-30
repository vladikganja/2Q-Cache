#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <chrono>
#include "2Qcache.hpp"

// imitation
size_t slow_get_page(size_t n) {
    return n;
}

size_t LRUhits(const std::vector<size_t>& values) {
    size_t m;
    std::cout << "enter LRU cache size: ";
    std::cin >> m;
    size_t hits = 0;
    cache::LRUcache<size_t> c(m);
    for (size_t i = 0; i < values.size(); i++) {
        size_t page = values[i];
        assert(std::cin.good());
        if (c.lookup_update(page, slow_get_page)) {
            hits++;
        }
    }
    return hits;
}

size_t TwoQhits(const std::vector<size_t>& values) {
    size_t m;
    std::cout << "enter 2Q-hot cache size: ";
    std::cin >> m;
    size_t hits = 0;
    cache::TwoQueueCache<size_t> c(m);
    for (size_t i = 0; i < values.size(); i++) {
        size_t page = values[i];
        assert(std::cin.good());
        if (c.lookup_update(page, slow_get_page)) {
            hits++;
        }
    }
    return hits;
}

int main() {

    size_t n;
    std::cout << "enter test size: ";
    std::cin >> n;

    std::mt19937 engine(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<size_t> gen(1, n / 4);
    std::vector<size_t> values;
    for (size_t i = 0; i < n; i++) {
        values.push_back(gen(engine));
    }

    std::cout << "hits: " << LRUhits(values) << std::endl;
    std::cout << "hits: " << TwoQhits(values) << std::endl;

    return 0;
}
