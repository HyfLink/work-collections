#include "eztl.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <list>

class Object
{
public:
    ~Object() { std::cout << " - dtor" << std::endl; }
    Object(int i = 0) : n(i) { std::cout << " + ctor" << std::endl; };
    Object(const Object &other) : n(other.n) { std::cout << "copy ctor" << std::endl; };
    Object(Object &&other) : n(other.n) { std::cout << "move ctor" << std::endl; };

    Object &operator=(const Object &other)
    {
        n = other.n;
        std::cout << "copy" << std::endl;
        return *this;
    }

    int get() { return n; }
    int inc() { return ++n; }
    int add(int rhs) const { return n + rhs; }
    operator int&() { return n; }

private:
    int n;
};

std::ostream &operator<<(std::ostream &os, Object &obj)
{
    return os << obj.get();
}

int main(void)
{
    Object l = 12;
    l = 12;

    return 0;
}
