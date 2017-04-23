//
// Created by bruno on 07/02/17.
//

#include "freeList.h"


void *freeList::allocate() {
    using namespace boost::icl;

    for (auto it = freeMemory.begin(); it != freeMemory.end(); ++it) {
        char* lower = it->lower();
        auto inter = memoryInterval::closed(lower, lower + objSize);
        if(contains(freeMemory, inter)){
            freeMemory.subtract(inter);
            return lower;
        }
    }
    return nullptr;
}

void freeList::free(void * ptr) {
    addMemory(static_cast<char*>(ptr), objSize);
}

bool freeList::canAllocate() {
    return !freeMemory.empty();
}

void freeList::addMemory(char *ptr, std::size_t size) {
    freeMemory.add(memoryInterval::closed(ptr, ptr + size));
}

void freeList::addMemory(memoryChunkHeader &chunk) {
    addMemory(chunk.startOfMemory(), chunk.getMemorySize());
}

