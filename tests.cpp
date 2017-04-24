#include "tests.h"
#include "Gc.h"

struct A {
    std::size_t x;
    A(std::size_t x) : x{x} {}
};

struct B {
    Gc<A> a;
    B(std::size_t x) : a{MakeGc<A>(x)} {}
};

struct C {
    Gc<B> b;
    char a[1024];
    C(std::size_t x) : b{MakeGc<B>(x)} {}
};

void test_small_allocations(std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        auto b = MakeRootGc<B>(i);
        for (std::size_t j = 0; j < 50; ++j) {
            b->a = MakeGc<A>(j);
        }
    }
}

void test_big_allocations(std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        auto c = MakeRootGc<C>(i);
        for (std::size_t j = 0; j < 50; ++j) {
            c->b = MakeGc<B>(j);
        }
    }
}

void test_realistic(std::size_t size) {

}

