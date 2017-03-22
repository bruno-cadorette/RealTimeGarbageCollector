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

    Gc(const Gc& other);

    template <bool OtherIsRoot>
    Gc(const Gc<T, OtherIsRoot> &other);


    Gc& operator=(const Gc &other);

    template <bool OtherIsRoot>
    Gc& operator=(const Gc<T, OtherIsRoot> &other);

    T& operator*() { return *get(); }
    const T& operator*() const { return *get(); }
    T* operator->() { return get(); }
    const T* operator-> () const { return get(); }


    encodedPtr _rawIndex() const { return ptrIndex; }

private:
    T* get() const { return static_cast<T*>(garbageCollector::get()[ptrIndex]); }

    void swap(Gc &other);
    void init();
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

template <class T, bool IsRoot>
Gc<T, IsRoot>::Gc(const Gc<T, IsRoot> &other)
    : ptrIndex(other.ptrIndex) {
    init();
}

template <class T, bool IsRoot>
template <bool OtherIsRoot>
Gc<T, IsRoot>::Gc(const Gc<T, OtherIsRoot> &other)
    : ptrIndex{other._rawIndex()} {
    init();
}

template <class T, bool IsRoot>
void Gc<T, IsRoot>::init() {
    if (IsRoot) {
        garbageCollector::get().addRoot(ptrIndex);
    }
}

template <class T, bool IsRoot>
void Gc<T, IsRoot>::swap(Gc<T, IsRoot> &other) {
    using std::swap;
    swap(ptrIndex, other.ptrIndex);
}


template <class T, bool IsRoot>
Gc<T, IsRoot>& Gc<T, IsRoot>::operator=(const Gc<T, IsRoot> &other) {
    Gc<T, IsRoot> me(other);
    swap(me);
    return *this;
}

template <class T, bool IsRoot>
template <bool OtherIsRoot>
Gc<T, IsRoot>& Gc<T, IsRoot>::operator=(const Gc<T, OtherIsRoot> &other) {
    Gc<T, IsRoot> me(other);
    swap(me);
    return *this;
}

#endif //REALTIMEGARBAGECOLLECTOR_GCPTR_H
