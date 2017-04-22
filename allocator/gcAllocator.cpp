//
// Created by bruno on 31/01/17.
//

#include "gcAllocator.h"

freeList &gcAllocator::getFreeList(std::size_t i) {
    return freeLists[i / 4 - 4];
}

void *gcAllocator::allocate(std::size_t alloc_size) {
    return isSmallObject(alloc_size) ? allocateSmall(alloc_size) : allocateBig(alloc_size);
}

bool gcAllocator::isSmallObject(std::size_t size) {
    return size < 4096;
}

void *gcAllocator::allocateSmall(std::size_t size) {
    auto lst = getFreeList(size);
    if(!lst.canAllocate()){
        //We can alloc more memory or do a collection right here
       /* memoryChunk m(size);
        lst.addMemory(m);
        chunks.push_back(std::move(m));*/
    }
    return range.newPrt(lst.allocate(), size);
}

void *gcAllocator::allocateBig(std::size_t size) {
    return nullptr;
}

bool gcAllocator::isValidPtr(char *ptr) {
    std::size_t address = reinterpret_cast<std::size_t>(ptr);
    if(range.inRange(address)){
        auto midIndex = gcIndex.getData(address);
        if(midIndex){
            auto chunkHeader = midIndex->getData(address);
            if(chunkHeader){
                return chunkHeader->isValid(ptr);
            }
        }
    }
    return false;
}

gcAllocator::gcAllocator() {
    freeLists.reserve(1024);
    for (std::size_t i = 0; i < 1024; ++i) {
        freeLists.emplace_back(i * 4 + 4);
    }
}
