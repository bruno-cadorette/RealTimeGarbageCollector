//
// Created by bruno on 21/01/17.
//

#include "garbageCollector.h"

#include <stack>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <cassert>

void garbageCollector::collect() {
    GcCollectMonitor collectStats{*this, stats};

    std::stack<char*> toMark;
    for (const auto& root : roots) {
        allocator.mark(root.first);
        toMark.push(root.first);
    }
    while (!toMark.empty()){
        char* hItem = toMark.top();
        toMark.pop();
        if(!allocator.isMarked(hItem)){
            allocator.mark(hItem);
            auto ptrs = allocator.innersPtr(hItem);
            std::for_each(ptrs.begin(), ptrs.end(), [&](char* x){
                toMark.push(x);
            });
        }
    }
    allocator.collectMemory();
}

bool garbageCollector::canAllocate(size_t objSize) {
    return currentHeapSize + objSize < MAX_HEAP_SIZE;
}

garbageCollector& garbageCollector::get() {
    static garbageCollector gc;
    return gc;
}

void garbageCollector::addRoot(char* item) {
    ++roots[item];
}
void garbageCollector::removeRoot(char* item) {
    auto it = roots.find(item);
    assert(it != std::end(roots));
    assert(it->second > 0);

    --it->second;

    if (it->second == 0) {
        roots.erase(it);
    }
}

void garbageCollector::_showState() {
    std::cout << "Roots: " << std::endl;
    for (const auto& root : roots) {
        std::cout << "  " << static_cast<void*>(root.first);
        std::cout << " (" << root.second << ")";
        std::cout << std::endl;
    }
    if (roots.empty()) std::cout << "  (empty)" << std::endl;
}

std::size_t garbageCollector::getMemoryOverhead() const {
    const auto our_size = sizeof(garbageCollector);

    // check size of an int heap item, but it's the same for any T.
    const auto heap_item_size = sizeof(heapItemImpl<int>);

    // root: encoded ptr + counter
    const auto root_size = sizeof(encodedPtr) + sizeof(size_t);

    return our_size +
           heap_item_size * heap.size() +
           root_size * roots.size();
}
