#pragma once
#include <unordered_map>
#include <list>

namespace cache {

// LRU - last recently used
template <typename ValT, typename KeyT = size_t>
class LRUcache {
private:
    size_t size_;
    std::list<ValT> cache_;
    using ListIt = typename std::list<ValT>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;

    bool full() const {
        return cache_.size() == size_;
    }

public:
    LRUcache(size_t size) : size_(size) {}

    template <typename Func>
    bool lookup_update(KeyT key, Func slow_get) {
        auto hit = hash_.find(key);
        if (hit == hash_.end()) {
            if (full()) {
                hash_.erase(cache_.back());
                cache_.pop_back();                
            }
            cache_.push_front(slow_get(key));
            hash_[key] = cache_.begin();
            return false;
        }

        auto eltit = hit->second;
        if (eltit != cache_.begin()) {
            cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
        }
        return true;
    }
};

}
