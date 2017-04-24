//
// Created by bruno on 21/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H
#define REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H

#include <map>
#include <algorithm>
#include "heapItem.h"
#include "encoding.h"
#include "GcStats.h"
#include "allocator/gcAllocator.h"

class garbageCollector {

    ptrSize currentHeapSize = 0;
    std::vector<heapItem*> heap;
    std::map<char*, std::size_t> roots;
    GcStats stats;
    gcAllocator allocator;
    bool canAllocate(size_t objSize);

public:
    template<class T, class... Args>
    T* allocate(Args&&... args) {
        auto ptr = allocator.allocate<T>([&] { collect(); });
        return new (ptr) T{std::forward<Args>(args)...};
    }
    void* operator[](const encodedPtr& ptr) {
        return heap[ptr.decode()]->getItem();
    }
    void addRoot(char* item);
    void removeRoot(char* item);

    void collect();

    static garbageCollector &get();


    const GcStats& getStats() const { return stats; }
    void resetStats() { stats = {}; }
    std::size_t getMemoryOverhead() const;
    std::size_t getManagedMemorySize() const { return currentHeapSize; }
    // Shows information about the GC state (for debug purposes)
    void _showState() const;
    void _showStats() const;
};



#endif //REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H
