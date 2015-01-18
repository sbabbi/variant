
#include <iostream>

#include "variant.hpp"
#include "visitor.hpp"

struct F
{
    template<class T>
    void operator()(const T & v) const
    {
        std::cout << v << std::endl;
    }
};

struct G
{
    template<class T, class U, class Z>
    void operator()(const T & v, const U & u, const Z & z) const
    {
        std::cout << v << ", " << u << ", " << z << std::endl;
    }
};

void f1()
{
    typedef variant<std::string, int, double> var_t;
    var_t x = 3.14;
    
    apply_visitor(F{}, x);
}

void f2()
{
    typedef variant<std::string, int, double> var_t;
    
    var_t x = "Hello, world!";
    var_t y = 2;
    var_t z = 3.14;

    apply_visitor(G{}, x, y, z);
}
