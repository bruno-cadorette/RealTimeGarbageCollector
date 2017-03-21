#include <iostream>
#include "Gc.h"

using namespace std;

struct B {
    int bar;

    B(int bar) : bar{bar} {}
};
struct A {
    int foo;
    Gc<B> b;

    A(int foo) : foo{foo}, b{new B{foo + 1}} {}
};

int main() {
    garbageCollector::get()._showState();
    cout << "Creating 'a'" << endl;
    RootGc<A> a = new A{5};
    garbageCollector::get()._showState();

    cout << "Creating 'b'" << endl;
    RootGc<A> b = new A{7};
    garbageCollector::get()._showState();
}
