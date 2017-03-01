//
// Created by bruno on 31/01/17.
//

#include "allocator.h"

freeList &allocator::getFreeList(std::size_t i) {
    return freeLists[(std::size_t)ceil(log2(i))];
}

void *allocator::allocate(std::size_t alloc_size) {
    return isSmallObject(alloc_size) ? allocateSmall(alloc_size) : allocateBig(alloc_size);
}

bool allocator::isSmallObject(std::size_t size) {
    return size < 4096;
}

void *allocator::allocateSmall(std::size_t size) {
    auto lst = getFreeList(size);
    if(!lst.canAllocate()){
        //We can alloc more memory or do a collection right here
       /* memoryChunk m(size);
        lst.addMemory(m);
        chunks.push_back(std::move(m));*/
    }
    return range.newPrt(lst.allocate(), size);
}

void *allocator::allocateBig(std::size_t size) {
    return nullptr;
}

bool allocator::isValidPtr(char *ptr) {
    std::size_t address = static_cast<std::size_t>(ptr);
    if(range.inRange(address)){
        auto midIndex = gcIndex.getData(address);
        if(midIndex){
            auto chunkHeader = midIndex->getData(address);
            if(chunkHeader){
                return chunkHeader->isValid(address);
            }
        }
    }
    return false;
}
