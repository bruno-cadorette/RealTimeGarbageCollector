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
struct C {
    Gc<A> a;

    C(const Gc<A>& a) : a{a} {}
};

int main() {
    {
        garbageCollector::get()._showState();
        cout << "Creating 'a'" << endl;
        RootGc<A> a = new A{5};
        garbageCollector::get()._showState();

        cout << "Creating 'b'" << endl;
        RootGc<A> b = new A{7};
        garbageCollector::get()._showState();

        cout << "Creating 'c'" << endl;
        RootGc<A> c = b;
        garbageCollector::get()._showState();

        /* cout << "Creating 'd'" << endl; */
        /* RootGc<A> d{a}; */
        /* garbageCollector::get()._showState(); */

        /* cout << "b = a" << endl; */
        /* b = a; */
        /* garbageCollector::get()._showState(); */

    /*     cout << "b.b = new B" << endl; */
    /*     b.b = new B{9}; */
    /*     garbageCollector::get()._showState(); */
        cout << "Dtors." << endl;
    }
    garbageCollector::get()._showState();
}
