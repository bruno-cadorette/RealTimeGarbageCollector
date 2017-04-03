//
// Created by bruno on 21/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H
#define REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H

#include <map>
#include <algorithm>
#include "heapItem.h"
#include "encoding.h"
class garbageCollector {

    ptrSize currentHeapSize = 0;
    std::vector<heapItem*> heap;
    std::map<std::size_t, std::size_t> roots;

    bool canAllocate(size_t objSize);

public:
    template<class T, class... Args>
    encodedPtr allocate(Args&&... args) {
        auto size = sizeof(T);
        if (!canAllocate(size))
        {
            collect();
            if (!canAllocate(size)) return 0; //TODO error handling
        }

        // TODO allocate in our custom heap
        auto item = new T{std::forward<Args>(args)...};

        currentHeapSize += size;
        auto hItem = new heapItemImpl<T>(item);
        heap.push_back(hItem);
        auto index = heap.size() - 1;
        return {index};
    }
    void* operator[](const encodedPtr& ptr) {
        return heap[ptr.decode()]->getItem();
    }
    void addRoot(encodedPtr item);
    void removeRoot(encodedPtr item);

    void collect();

    static garbageCollector &get();


    // Shows information about the GC state (for debug purposes)
    void _showState();
};



#endif //REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H
