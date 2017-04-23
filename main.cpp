#include <iostream>
#include <iomanip>
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
    cout << "Variables are all deads" << endl;
    garbageCollector::get()._showState();
    garbageCollector::get().collect();

    cout << "Collection stats: " << endl;
    const auto stats = garbageCollector::get().getStats();
    const auto time = stats.getTimeStats();
    const auto memory = stats.getMemoryStats();
    using chrono::duration_cast;
    using ns = chrono::nanoseconds;
    cout << std::fixed << std::setprecision(2);
    cout << "      Total collections: "
         << time.count()
         << endl

         << "           Average time: "
         << duration_cast<ns>(time.avg()).count() << "ns"
         << endl

         << "           Maximum time: "
         << duration_cast<ns>(time.max()).count() << "ns"
         << endl

         << "     Busted constraints: "
         << stats.getBustedTimeConstraints()
         << endl

         << "Average memory overhead: "
         << memory.avg() * 100.f << "%"
         << endl

         << "Minimum memory overhead: "
         << memory.min() * 100.f << "%"
         << endl;
}
