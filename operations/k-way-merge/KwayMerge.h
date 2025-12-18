#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <filesystem>
#include <queue>
#include <algorithm>

template<typename T>
struct MergeNode {
    T value;
    size_t array_index;
    size_t element_index;
    
    bool operator>(const MergeNode& other) const {
        return value > other.value;
    }
};

template<typename T>
std::vector<T> mergeKBlocks(const std::vector<std::vector<T>>& blocks) {
    if (blocks.empty()) return {};
    if (blocks.size() == 1) return blocks[0];
    
    std::priority_queue<
        MergeNode<T>,
        std::vector<MergeNode<T>>,
        std::greater<MergeNode<T>>
    > min_heap;
    
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (!blocks[i].empty()) {
            min_heap.push({blocks[i][0], i, 0});
        }
    }
    
    std::vector<T> result;
    size_t total_size = 0;
    for (const auto& block : blocks) {
        total_size += block.size();
    }
    result.reserve(total_size);
    
    while (!min_heap.empty()) {
        MergeNode<T> current = min_heap.top();
        min_heap.pop();
        
        result.push_back(current.value);
        
        size_t next_idx = current.element_index + 1;
        if (next_idx < blocks[current.array_index].size()) {
            min_heap.push({
                blocks[current.array_index][next_idx],
                current.array_index,
                next_idx
            });
        }
    }
    
    return result;
}

template<typename T>
std::vector<T> KwayMergeSort(const std::vector<T>& array
    , std::size_t k = 4
    , std::size_t block_size = 8)
{
    /*TODO*/
    if (array.empty()) {
        return std::vector<T>();
    }
    
    if (k < 2) {
        k = 2;
    }
    if (block_size == 0) {
        block_size = 1;
    }
    
    std::vector<std::vector<T>> sorted_blocks;
    
    for (size_t i = 0; i < array.size(); i += block_size) {
        size_t end = std::min(i + block_size, array.size());
        std::vector<T> block(array.begin() + i, array.begin() + end);
        std::sort(block.begin(), block.end());
        sorted_blocks.push_back(std::move(block));
    }
    
    if (sorted_blocks.size() == 1) {
        return sorted_blocks[0];
    }
    
    k = std::min(k, sorted_blocks.size());
    
    std::vector<std::vector<T>> current_blocks = sorted_blocks;
    
    while (current_blocks.size() > 1) {
        std::vector<std::vector<T>> merged_blocks;
        
        for (size_t i = 0; i < current_blocks.size(); i += k) {
            size_t group_end = std::min(i + k, current_blocks.size());
            
            std::vector<std::vector<T>> group_to_merge(
                current_blocks.begin() + i,
                current_blocks.begin() + group_end
            );
            
            std::vector<T> merged = mergeKBlocks(group_to_merge);
            merged_blocks.push_back(std::move(merged));
        }
        
        current_blocks = std::move(merged_blocks);
    }
    
    return current_blocks[0];
}

