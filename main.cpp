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
    cout << "Creating 'a'" << endl;
    auto a = MakeRootPtr<A>(5);

    cout << "Creating 'b'" << endl;
    auto b = MakeRootPtr<A>(7);

}
