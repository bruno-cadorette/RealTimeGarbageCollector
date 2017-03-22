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
    std::stack<heapItem*> toMark;
    for (const auto& root : roots) {
        toMark.push(heap[root.first]);
    }
    while (!toMark.empty()){
        auto hItem = toMark.top();
        toMark.pop();
        if(!hItem->marked){
            hItem->marked = true;
            auto ptrs = hItem->innerPtrs();
            std::for_each(ptrs.begin(), ptrs.end(), [&](uint64_t x){
                toMark.push(heap.at(x));
            });
        }
    }

    heap.erase(std::remove_if(heap.begin(), heap.end(), [&](heapItem* h){
        if(h->marked){
            h->marked = false;
            return false;
        }
        else{
            currentHeapSize -= h->getItemSize();
            delete h;
            return true;
        }
    }), heap.end());
}

bool garbageCollector::canAllocate(size_t objSize) {
    return currentHeapSize + objSize < MAX_HEAP_SIZE;
}

garbageCollector& garbageCollector::get() {
    static garbageCollector gc;
    return gc;
}

void garbageCollector::addRoot(encodedPtr item) {
    const auto ptr = item.decode();
    ++roots[ptr];
}
void garbageCollector::removeRoot(encodedPtr item) {
    const auto ptr = item.decode();
    auto it = roots.find(ptr);
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
        std::cout << "  " << root.first;
        std::cout << " (" << root.second << ")";
        std::cout << std::endl;
    }
    if (roots.empty()) std::cout << "  (empty)" << std::endl;
}
