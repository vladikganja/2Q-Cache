#pragma once
#include "LRUcache.hpp"

namespace cache {

// 2Q - Two queues + LRU
template <typename ValT, typename KeyT = size_t>
class TwoQueueCache {
private:

    size_t size_in_;
    size_t size_out_;
    size_t size_hot_;

    std::list<ValT> in_;
    std::list<ValT> out_;
    std::list<ValT> hot_;

    using ListIt = typename std::list<ValT>::iterator;

    std::unordered_map<KeyT, ListIt> hash_in_;
    std::unordered_map<KeyT, ListIt> hash_out_;
    std::unordered_map<KeyT, ListIt> hash_hot_;

    bool hot_full() const {
        return hot_.size() == size_hot_;
    }

    template <typename KeyT, typename Func>
    void insert_new(const KeyT& key, Func get_data) {
        if (in_.size() == size_in_) {
            if (out_.size() == size_out_) {
                hash_out_.erase(out_.back());
                out_.pop_back();
            }
            out_.push_front(in_.back());
            hash_out_[*out_.begin()] = out_.begin();
            hash_in_.erase(in_.back());
            in_.pop_back();
        }
        in_.push_front(get_data(key));
        hash_in_[key] = in_.begin();
    }

    void insert_from_out_to_hot(const KeyT& key) {
        if (hot_full()) {
            hash_hot_.erase(hot_.back());
            hot_.pop_back();
        }
        hot_.push_front(key);
        hash_hot_[key] = hot_.begin();
    }

public:
    // proportion hot/in/out = 25%/25%/50%
    TwoQueueCache(size_t hot_size) : size_in_(hot_size), size_hot_(hot_size), size_out_(hot_size * 2) {}

    template <typename KeyT, typename Func>
    bool lookup_update(const KeyT& key, Func get_data) {
        auto hit = hash_hot_.find(key);
        if (hit != hash_hot_.end()) {
            auto eltit = hit->second;
            if (eltit != hot_.begin()) {
                hot_.splice(hot_.begin(), hot_, eltit, std::next(eltit));
            }
            return true;
        }
        else {
            auto hit_in = hash_in_.find(key);
            if (hit_in != hash_in_.end()) {
                return true;
            }
            else {
                auto hit_out = hash_out_.find(key);
                if (hit_out != hash_out_.end()) {
                    insert_from_out_to_hot(key);
                    return true;
                }
                else {
                    insert_new(key, get_data);
                    return false;
                }
            }
        }
    }

    void debug_print() {
        std::cout << "IN:\t";
        for (auto el : in_) {
            std::cout << el << ' ';
        }
        std::cout << std::endl << "OUT:\t";
        for (auto el : out_) {
            std::cout << el << ' ';
        }
        std::cout << std::endl << "HOT:\t";
        for (auto el : hot_) {
            std::cout << el << ' ';
        }
        std::cout << std::endl;
    }
};

}
