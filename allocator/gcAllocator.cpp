//
// Created by bruno on 31/01/17.
//

#include "gcAllocator.h"

freeList &gcAllocator::getFreeList(std::size_t i) {
    return freeLists[i / 4 - 4];
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

std::vector<char*> gcAllocator::innersPtr(char* ptr) {
    std::vector<char*> ptrs;
    std::size_t address = reinterpret_cast<std::size_t>(ptr);
    if(range.inRange(address)){
        auto midIndex = gcIndex.getData(address);
        if(midIndex){
            auto chunkHeader = midIndex->getData(address);
            if(chunkHeader && chunkHeader->isValid(ptr)){
                auto size = chunkHeader->getPtrSize();
                for (int i = 1; i < size - sizeof(size_t); ++i) {
                    if(isValidPtr(ptr + i)){
                        ptrs.push_back(ptr + i);
                    }
                }
            }
        }
    }
    return ptrs;
}

gcAllocator::gcAllocator() {
    freeLists.reserve(1024);
    for (std::size_t i = 0; i < 1024; ++i) {
        freeLists.emplace_back(i * 4 + 4);
    }
}
