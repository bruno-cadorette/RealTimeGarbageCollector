//
// Created by bruno on 31/01/17.
//

#include "allocator.h"

freeList &allocator::getFreeList(std::size_t i) {
    return freeLists.at((std::size_t)ceil(log2(i)));
}

void *allocator::allocate(std::size_t alloc_size) {
    auto lst = getFreeList(alloc_size);
    if(!lst.canAllocate()){
        memoryChunk m(alloc_size);
        lst.addMemory(m);
        chunks.push_back(std::move(m));
    }
    return lst.allocate();
}
