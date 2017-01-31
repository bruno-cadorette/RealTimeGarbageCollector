//
// Created by bruno on 22/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_HEAPITEM_H
#define REALTIMEGARBAGECOLLECTOR_HEAPITEM_H
#include <vector>
#include <cstdint>
#include "encoding.h"

using ptrSize = std::size_t;
struct heapItem{

    bool marked;
    heapItem(){}
    heapItem(bool marked) : marked{marked}{}
    virtual void* getItem() = 0;
    virtual ptrSize getItemSize() = 0;
    virtual std::vector<uint64_t> innerPtrs() = 0;
    virtual ~heapItem(){};
};

template <class T>
struct heapItemImpl : heapItem {
    T *item;
    ptrSize getItemSize(){
        return sizeof(item);
    }
    heapItemImpl(T *item) :
            heapItem(false), item{item} {
    }
    void* getItem(){
        return (void*)item;
    }
    std::vector<uint64_t> innerPtrs() {
        std::vector<uint64_t> v;
        auto objSize = sizeof(*item);
        auto ptrSize = sizeof(uint64_t);
        auto bytes = reinterpret_cast<char *>(item);
        if (bytes != nullptr) {
            for (int i = 0; i < objSize - ptrSize; ++i) {
                auto x = *((uint64_t *) bytes + i);
                if (encoding::isValidPtr(x)) {
                    v.push_back(encoding::decode(x));
                }
            }
            return v;
        }
    }

    ~heapItemImpl() {
        delete item;
    }
};

#endif //REALTIMEGARBAGECOLLECTOR_HEAPITEM_H
