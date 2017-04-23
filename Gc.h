#ifndef REALTIMEGARBAGECOLLECTOR_GCPTR_H
#define REALTIMEGARBAGECOLLECTOR_GCPTR_H

#include "garbageCollector.h"

#include <utility>

template <class T, bool IsRoot>
class Gc;

template <class T>
using RootGc = Gc<T, true>;

template <class T, class... Args>
Gc<T, false> MakeGc(Args&&... args);
template <class T, class... Args>
RootGc<T> MakeRootGc(Args&&... args);


template <class T, bool IsRoot=false>
class Gc {
    T* ptr;
public:
    Gc(const Gc& other);

    template <bool OtherIsRoot>
    Gc(const Gc<T, OtherIsRoot> &other);

    ~Gc();

    Gc& operator=(const Gc &other);

    template <bool OtherIsRoot>
    Gc& operator=(const Gc<T, OtherIsRoot> &other);

    T& operator*() { return *get(); }
    const T& operator*() const { return *get(); }
    T* operator->() { return get(); }
    const T* operator-> () const { return get(); }


    T* _rawIndex() const { return ptr; }

private:
    Gc(T* ptr);

    template <class U, class... Args>
    friend Gc<U> MakeGc(Args&&...);

    template <class U, class... Args>
    friend RootGc<U> MakeRootGc(Args&&...);

    T* get() const { return ptr; }

    void swap(Gc &other);
    void init();
};

template <class T, class... Args>
Gc<T> MakeGc(Args&&... args) {
    auto ptrIndex = garbageCollector::get().allocate<T>(std::forward<Args>(args)...);
    return Gc<T>{ptrIndex};
}
template <class T, class... Args>
RootGc<T> MakeRootGc(Args&&... args) {
    auto ptrIndex = garbageCollector::get().allocate<T>(std::forward<Args>(args)...);
    return RootGc<T>{ptrIndex};
}

template <class T, bool IsRoot>
Gc<T, IsRoot>::Gc(T* ptr) : ptr{ptr} {
    init();
}

template <class T, bool IsRoot>
Gc<T, IsRoot>::~Gc() {
    if (IsRoot) { // non-roots are handled by the GC
        garbageCollector::get().removeRoot(reinterpret_cast<char*>(ptr));
    }
}

template <class T, bool IsRoot>
Gc<T, IsRoot>::Gc(const Gc<T, IsRoot> &other)
    : ptr(other.ptr) {
    init();
}

template <class T, bool IsRoot>
template <bool OtherIsRoot>
Gc<T, IsRoot>::Gc(const Gc<T, OtherIsRoot> &other)
    : ptr{other._rawIndex()} {
    init();
}

template <class T, bool IsRoot>
void Gc<T, IsRoot>::init() {
    if (IsRoot) {
        garbageCollector::get().addRoot(reinterpret_cast<char*>(ptr));
    }
}

template <class T, bool IsRoot>
void Gc<T, IsRoot>::swap(Gc<T, IsRoot> &other) {
    using std::swap;
    swap(ptr, other.ptr);
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
