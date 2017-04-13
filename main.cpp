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

    A(const Gc<B>& b) : foo {0}, b{b} {}
    A(int foo) : foo{foo}, b{MakeGc<B>(foo + 1)} {}
};

int main() {
    {
        garbageCollector::get()._showState();
        cout << "Creating 'a'" << endl;
        auto a = MakeRootGc<A>(5);
        garbageCollector::get()._showState();

        cout << "Creating 'b'" << endl;
        auto b = MakeRootGc<A>(7);
        garbageCollector::get()._showState();

        cout << "Creating 'c' equal to b" << endl;
        RootGc<A> c = b;
        garbageCollector::get()._showState();

        cout << "Creating 'd' equal to b->b" << endl;
        RootGc<B> d{b->b};
        garbageCollector::get()._showState();

        cout << "b = a" << endl;
        b = a;
        garbageCollector::get()._showState();

        cout << "b.b = new B" << endl;
        b->b = MakeGc<B>(9);
        garbageCollector::get()._showState();

        cout << "d = b->b" << endl;
        d = b->b;
        garbageCollector::get()._showState();

        cout << "b->b = d" << endl;
        b->b = d;
        garbageCollector::get()._showState();

        cout << "Dtors." << endl;
    }
    garbageCollector::get()._showState();
    garbageCollector::get().collect();

    cout << "Collection times: " << endl;
    const auto stats = garbageCollector::get().getStats();
    for (auto& duration : stats.getCollectDurations()) {
        auto ns = chrono::duration_cast<chrono::nanoseconds>(duration).count();
        cout << ns << "ns" << endl;
    }
}
