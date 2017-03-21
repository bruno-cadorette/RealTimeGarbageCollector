#ifndef REALTIMEGARBAGECOLLECTOR_GCPTR_H
#define REALTIMEGARBAGECOLLECTOR_GCPTR_H

#include "garbageCollector.h"

#include <utility>

template <class T, bool IsRoot>
class Gc;

template <class T>
using RootGc = Gc<T, true>;

template <class T, class... Args>
RootGc<T> MakeRootPtr(Args&&... args);


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
    bool root() const { return IsRoot; }
};



template <class T, bool IsRoot>
Gc<T, IsRoot>::Gc(T* ptr) {
    //TODO only add to roots set here if root
    ptrIndex = garbageCollector::get().allocate(ptr);
}

template <class T, bool IsRoot>
Gc<T, IsRoot>::~Gc() {
    if (root()) { // non-roots are handled by the GC
        garbageCollector::get().removeReference(ptrIndex);
    }
}

template <class T, class... Args>
RootGc<T> MakeRootPtr(Args&&... args) {
    return {new T{std::forward<Args>(args)...}};
}

#endif //REALTIMEGARBAGECOLLECTOR_GCPTR_H
