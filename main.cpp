#include <iostream>
#include <new>
#include <memory>
#include <algorithm>
#include "allocator/freeList.h"
#include "allocator/gcAllocator.h"

using namespace std;

gcAllocator alloc;

void treetest(){
    gcTopIndex a;
    gcBottomIndex b;
    const size_t ptr = 4096;
    cout << a.getIndexFromPtr(ptr) << endl;
    cout << b.getIndexFromPtr(ptr) << endl;
}

void allocatorTest(){
    for (int i = 0; i < 100000; ++i) {
        auto a = alloc.allocate<int>();
        *a = 9;
        assert(alloc.isValidPtr(reinterpret_cast<char*>(a)));
    }

    assert(!alloc.isValidPtr(reinterpret_cast<char*>(0x7286424)));

}

int main() {
    allocatorTest();
    treetest();
}


