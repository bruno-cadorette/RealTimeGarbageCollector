//
// Created by bruno on 07/02/17.
//

#include "freeList.h"

void *freeList::allocate() {
    using namespace boost::icl;
    auto it = freeMemory.begin();
    char* lower = it->lower();
    auto inter = memoryInterval::closed(lower, lower + objSize);
    if(contains(freeMemory, inter)){
        freeMemory.subtract(inter);
        return lower;
    }
    else{
        //this shouldn't happen, it means that the first pointer range is lower than an allocatable object.
        // I don't know what to do
        //Delete from the list and free ->
    }
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

