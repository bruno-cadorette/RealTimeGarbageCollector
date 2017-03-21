#ifndef REALTIMEGARBAGECOLLECTOR_GCPTR_H
#define REALTIMEGARBAGECOLLECTOR_GCPTR_H

#include "garbageCollector.h"

#include <utility>

template <class T, bool IsRoot>
class Gc;

template <class T>
using RootGc = Gc<T, true>;


template<class T, bool IsRoot=false>
class Gc {
    encodedPtr ptrIndex;

public:
    Gc(T* ptr = nullptr);
    ~Gc();

    T& operator*() { return *get(); }
    const T& operator*() const { return *get(); }
    T* operator->() { return get(); }
    const T* operator-> () const { return get(); }

private:
    T* get() const { return static_cast<T*>(garbageCollector::get()[ptrIndex]); }
};



template <class T, bool IsRoot>
Gc<T, IsRoot>::Gc(T* ptr) {
    ptrIndex = garbageCollector::get().allocate(ptr);

    if (IsRoot) {
        garbageCollector::get().addRoot(ptrIndex);
    }
}

template <class T, bool IsRoot>
Gc<T, IsRoot>::~Gc() {
    if (IsRoot) { // non-roots are handled by the GC
        garbageCollector::get().removeRoot(ptrIndex);
    }
}

#endif //REALTIMEGARBAGECOLLECTOR_GCPTR_H
