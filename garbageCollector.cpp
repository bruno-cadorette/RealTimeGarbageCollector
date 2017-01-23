//
// Created by bruno on 21/01/17.
//

#include "garbageCollector.h"

#include <stack>

void garbageCollector::collect() {
    std::stack<heapItem*> toMark;
    std::for_each(roots.begin(), roots.end(), [&](std::size_t i){
        toMark.push(heap[i]);
    });
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
    return currentHeapSize + objSize < encoding::MAX_HEAP_SIZE;
}

garbageCollector& garbageCollector::get() {
    static garbageCollector gc;
    return gc;
}
