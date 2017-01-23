#include <iostream>
#include "garbageCollector.h"
#include "gcPtr.h"
using namespace std;

struct test{
    gcPtr<test> b;
    gcPtr<char> c;
    gcPtr<std::string> d;
    char z;
    int a;
};

void ptrInfo(const test& x){
    std::cout<< "sizeof " << sizeof(x) << std::endl
             << (&x == (const test*)&(x.b)) << std::endl
             << "a " << &x.a << " "  << sizeof(x.a) << std::endl
             << "b " <<hex << x.b.ptrIndex << " " << sizeof(x.b) << std::endl
             << "c " << &x.c << " " << sizeof(x.c) << std::endl;
}

int main() {
    ptrInfo(test());
    test* b = new test;
    b->z = 8;
    b->a = 921;
    b->b = gcPtr<test>();
    b->c = gcPtr<char>();
    b->d = gcPtr<std::string>(new std::string("allo"));
    {
        auto a = new test;
        a->z = 2;
        a->a = 0xA3;
        a->b = gcPtr<test>(b);
        a->c = gcPtr<char>();
        a->d = gcPtr<std::string>(new std::string("allo"));
        gcPtr<test>z (a);
    }
    garbageCollector::get().collect();
    return 0;
}