//
// Created by bruno on 21/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H
#define REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H


#include <vector>
#include <unordered_set>
#include <algorithm>
#include "heapItem.h"
#include "encoding.h"
class garbageCollector {

    ptrSize currentHeapSize = 0;
    std::vector<heapItem*> heap;
    std::vector<std::size_t> roots;

    bool canAllocate(size_t objSize);

public:
    template<class T>
    encodedPtr allocate(T *item){
        auto size = sizeof(item);
        if (canAllocate(size))
        {
            currentHeapSize += size;
            auto hItem = new heapItemImpl<T>(item);
            heap.push_back(hItem);
            auto index = heap.size() - 1;
            roots.push_back(index);
            return encodedPtr{index};
        }
        else
        {
            collect();
            if(!canAllocate(size))
            {
                return 0;
            }
            allocate(item);
        }
    }
    void* operator[](const encodedPtr& ptr) {
        return heap[ptr.decode()]->getItem();
    }
    void removeReference(encodedPtr item) {
        auto ptr = item.decode();
        roots.erase(std::remove_if(roots.begin(), roots.end(), [&](std::uint64_t x){
            return ptr == x;
        }), roots.end());
    }

    void collect();

    static garbageCollector &get();
};



#endif //REALTIMEGARBAGECOLLECTOR_GARBAGECOLLECTOR_H
