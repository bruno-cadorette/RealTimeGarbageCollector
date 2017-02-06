#include <iostream>
#include "garbageCollector.h"
#include "gcPtr.h"
#include "allocator/memoryChunk.h"

using namespace std;
template <int N>
struct foo{};

int main() {
    uint32_t a = 3519029796;
    u_int16_t b = a >> 22;

    cout << b;
}