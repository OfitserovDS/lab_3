#pragma once
#include "ICache.h"
#include <unordered_map>
#include <list>
#include "stdexcept"

template<typename Key, typename Value>
class LRUCache : public ICache<Key, Value> {
private:
    struct CacheNode {
        Key key;
        Value value;
        
        CacheNode(const Key& k, const Value& v) : key(k), value(v) {}
    };
    
    std::size_t capacity_;
    std::list<CacheNode> usage_list_;
    std::unordered_map<Key, typename std::list<CacheNode>::iterator> cache_map_;

    void moveToFront(const Key& key) {
        auto it = cache_map_.find(key);
        if (it != cache_map_.end()) {
            usage_list_.splice(usage_list_.begin(), usage_list_, it->second);
        }
    }
    
public:
    explicit LRUCache(std::size_t capacity = 10) : capacity_(capacity) {
        if (capacity_ == 0) {
            throw std::invalid_argument("Invallid cache capacity");
        }
    }

    void set(const Key& key, const Value& value) override {
        auto it = cache_map_.find(key);
        
        if (it != cache_map_.end()) {
            it->second->value = value;
            moveToFront(key);
        } 
        else {
            if (size() >= capacity_) {
                auto last = usage_list_.back();
                cache_map_.erase(last.key);
                usage_list_.pop_back();
            }
            usage_list_.emplace_front(key, value);
            cache_map_[key] = usage_list_.begin();
        }
    }
    
    Value get(const Key& key) override {
        auto it = cache_map_.find(key);
        if (it == cache_map_.end()) {
            throw std::out_of_range("No key in cache");
        }
        moveToFront(key);

        return it->second->value;
    }
    
    bool contains(const Key& key) const override {
        return cache_map_.find(key) != cache_map_.end();
    }

    void clear() override {
        cache_map_.clear();
        usage_list_.clear();
    }
    
    std::size_t size() const override {
        return cache_map_.size();
    }
    
    std::size_t capacity() const {
        return capacity_;
    }
    
    ~LRUCache() override = default;
};