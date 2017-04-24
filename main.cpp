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
    for (size_t size = 1000; size <= 10000; size += 1000) {
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
    f(size);
    garbageCollector::get()._showStats();
}
