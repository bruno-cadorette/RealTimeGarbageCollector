#include <iostream>
#include <new>
#include <memory>
#include <algorithm>
#include "allocator/freeList.h"

using namespace std;


int main() {
    freeList a{1};
    memoryChunk b;
    a.addMemory(b);
    auto ptr = (char*)a.allocate();
    *ptr = 'b';
    cout << ((b.startOfMemory() == ptr) &&  (*b.startOfMemory() == *ptr));
}