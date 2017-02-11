//
// Created by bruno on 07/02/17.
//

#include "freeList.h"
#include <cmath>
#include <algorithm>

std::size_t freeList::getIndex(std::size_t i) {
    return (std::size_t)ceil(log2(i));
}

void *freeList::allocate() {
    auto item = freeMemory.front();
    auto ptr = item.allocate(objSize);
    if(ptr.second){
        freeMemory.pop_front();
    }
    return ptr.first;
}

void freeList::free(void *ptr) {
    auto it = std::find_if(freeMemory.begin(), freeMemory.end(), [&ptr](const freeListItem& x){
        return x.canMergePtr(ptr);
    });
    if(it == freeMemory.end()){
        //We can do better than n(log n)
        freeMemory.push_front(freeListItem((char*)ptr, objSize));
        freeMemory.sort([](freeListItem a, freeListItem b){
           return a.startOfAllocable - b.startOfAllocable;
        });
    }
    else{
        it->addPtr(objSize);
    }
}

bool freeList::canAllocate() {
    return !freeMemory.empty();
}

void freeList::addMemory(void *ptr, std::size_t size) {
    freeMemory.push_front(freeListItem((char*)ptr, size));
    freeMemory.sort([](freeListItem a, freeListItem b){
        return a.startOfAllocable - b.startOfAllocable;
    });
}

void freeList::addMemory(memoryChunk &chunk) {
    addMemory(chunk.startOfMemory(), chunk.getMemorySize());
}

