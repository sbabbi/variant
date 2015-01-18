# Variant

This variant class is largely inspired by [Boost.Variant](http://www.boost.org/doc/libs/1_57_0/doc/html/variant.html). It is written in C++14, making the code significantly more compact than the boost version.

Like [Boost.Variant](http://www.boost.org/doc/libs/1_57_0/doc/html/variant.html) this class possess the *never empty guarantee*, that is exactly one of the templated argument types will be active at any time.

Main differences with [Boost.Variant](http://www.boost.org/doc/libs/1_57_0/doc/html/variant.html)
   - Copy/move assignments with strong exception guarantee without using heap allocation.
   - `apply_visitor` uses variadic templates, and support mixing variant and non variant types.
   - No need for `make_recursive_variant` (see below).
 
### Copy and move assignments

Those familiar with <a href=http://www.boost.org/doc/libs/1_57_0/doc/html/variant.html>Boost.Variant</a> will know the main problem in the implementation of the copy assignment operator of a variant, that is ensuring the *never empty guarantee*. The problem is described better than I could [here](http://www.boost.org/doc/libs/1_57_0/doc/html/variant/design.html#variant.design.never-empty). The actual Boost solution makes use of heap allocation, that is quite inefficient.

My solution *requires* that at least all the types that might be containted in the variant are *NothrowMoveConstructible*. It simply create a temporary copy of the right hand side, and only after such copy is performed successfully it calls the old object destructor and *moves* the temporary in the variant storage. The overhead is only a move constructor. If one of the type contained in the variable is not `NothrowMoveConstructible` any use of the copy/move assignment will fail to compile.

### Visiting

This class supports extensive visit operation with the variadic function `apply_vistior`

    variant<vector<int>, string, int> x = {1,2,3,4};
    variant<char, int> y = 4;
    
    apply_visitor(f, x, y); //Calls f( vector<int>, int )
    apply_visitor(f, 15, x); //Calls f(int, vector<int> )
    apply_visitor(f, 12, 'a'); //Calls f(int, char)

 `apply_visitor` uses perfect forwarding to call the right overloaded `operator()` provided in the callable, and deduces the `noexcept` specification using the `noexcept()` operator on the provided `Callable`. Unlike [Boost.Variant](http://www.boost.org/doc/libs/1_57_0/doc/html/variant/design.html#variant.design.never-empty), the return type of the visitor is automatically deduced to the `std::common_type` of the return type of every  `operator()` that can be invoked by `apply_visitor`.
 
### Recursive variant
 
 The recommended way to implement recursive variant is as follows:

    struct X;
    typedef vector<X> vec_x;
    struct X : variant< vec_x, int > { using variant<vec_x, int>::variant; };

### Documentation

 Doxygen doc is [here](http://sbabbi.github.io/variant/index.html)

### Notes
This is a stub implementation, critics and improvements are welcome. I only tested it on clang with `-std=c++1y`. Both `libstdc++` and `libc++` seems to be compatible (if you want to run the tests with the latter you need a version of `googletest` compiled with `libc++`).

