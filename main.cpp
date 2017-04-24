#include <iostream>
#include "Gc.h"
#include "tests.h"

using namespace std;


template <class Func>
void evaluate(size_t size, Func f);


int main() {
    for (size_t size = 1000; size <= 10000; size += 1000) {
        cout << size << " small allocations:" << endl;
        evaluate(size, test_small_allocations);
        cout << endl;
    }

    for (size_t size = 100; size <= 1000; size += 100) {
        cout << size << " big allocations:" << endl;
        evaluate(size, test_big_allocations);
        cout << endl;
    }

    cout << "CSCoins miner:" << endl;
    evaluate(0, test_realistic);


    return 0;
}

template <class Func>
void evaluate(size_t size, Func f) {
    garbageCollector::get().resetStats();

    const auto start = StatsClock::now();
    f(size);
    const auto end = StatsClock::now();
    const auto total = end - start;
    const auto gc_total = garbageCollector::get().getStats().getTimeStats().total();

    const auto ratio = static_cast<float>(gc_total.count()) / total.count();

    garbageCollector::get()._showStats();
    cout << "  Time spent collecting: " << (ratio * 100.f) << "%" << endl;
}
