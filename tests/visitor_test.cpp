
#include <string>

#include "visitor.hpp"
#include "variant.hpp"
#include "type_name.hpp"
#include "gtest/gtest.h"

struct refprint
{
    template<class T>
    std::string operator()(const T &) const & noexcept( std::is_nothrow_copy_constructible<T>::value )
    { 
        return "refprint(" + type_name<const T &>() + ") const &"; 
    }
    
    template<class T>
    std::string operator()(const T &) && noexcept( std::is_nothrow_copy_constructible<T>::value )
    { 
        return "refprint(" + type_name<const T &>() + ") &&"; 
    }
    
    template<class T, class = typename std::enable_if< std::is_rvalue_reference<T&&>::value>::type >
    std::string operator()(T &&) const & noexcept( std::is_nothrow_move_constructible<T>::value )
    { 
        return "refprint(" + type_name<T &&>() + ") const &"; 
    }

    template<class T, class = typename std::enable_if< std::is_rvalue_reference<T&&>::value>::type >
    std::string operator()(T &&) && noexcept( std::is_nothrow_move_constructible<T>::value )
    { 
        return "refprint(" + type_name<T &&>() + ") &&"; 
    }
};

TEST(visitor, is_nothrow_visitable)
{
    typedef variant< std::string, int> var_t;

    EXPECT_TRUE((is_nothrow_visitable<refprint, int>::value ));
    EXPECT_TRUE(( is_nothrow_visitable<refprint, std::string>::value ));
    EXPECT_FALSE(( is_nothrow_visitable<refprint, std::string&>::value ));
    
    EXPECT_TRUE(( is_nothrow_visitable<refprint, var_t>::value ));
    EXPECT_FALSE(( is_nothrow_visitable<refprint, var_t&>::value ));
    EXPECT_FALSE(( is_nothrow_visitable<refprint, var_t const &>::value ));
}

TEST(visitor, visitor_ref)
{
    typedef variant< std::string, int> var_t;
    var_t a = "Hello";
    var_t b = 22;
    refprint print;
    
    EXPECT_EQ( "refprint(int const &) const &", apply_visitor( print, b) );
    EXPECT_EQ( "refprint(std::string const &) const &", apply_visitor( print, a ) );
    
    EXPECT_EQ( "refprint(int const &) &&", apply_visitor( refprint{}, b) );
    EXPECT_EQ( "refprint(std::string const &) &&", apply_visitor( refprint{}, a ) );

    EXPECT_EQ( "refprint(int &&) const &", apply_visitor( print, var_t(4)) );
    EXPECT_EQ( "refprint(std::string &&) const &", apply_visitor( print, var_t("hello") ) );

    EXPECT_EQ( "refprint(int &&) &&", apply_visitor( refprint{}, var_t(4) ) );
    EXPECT_EQ( "refprint(std::string &&) &&", apply_visitor( refprint{}, var_t("hello") ) );
}

TEST(visitor, visitor_ref_noexcept)
{
    typedef variant< std::string, int> var_t;
    var_t a = "Hello";
    refprint print;
    
    static_assert(!std::is_nothrow_copy_constructible<std::string>::value,"");
    static_assert(std::is_nothrow_move_constructible<std::string>::value,"");
    static_assert( noexcept(refprint{}), "");

    EXPECT_TRUE( noexcept( print(std::declval<int &>()) ) );
    EXPECT_FALSE( noexcept( print(std::declval<std::string &>()) ) );
    
    EXPECT_FALSE( noexcept(apply_visitor( print, a) ) );
    EXPECT_FALSE( noexcept(apply_visitor( refprint{}, a ) ) );
    EXPECT_TRUE( noexcept(apply_visitor( print, std::move(a) ) ) );
    EXPECT_TRUE( noexcept(apply_visitor( refprint{}, std::move(a) ) ) );
}

TEST(visitor, multivisitor)
{
    typedef variant< std::string, int> var_t;
    var_t a = "Hello";
    var_t b = 22;
    
    auto f = [](auto l, auto r) -> std::string 
    { 
        return static_cast<std::ostringstream&&>( std::ostringstream() << l << ' ' << r ).str();
    };
    
    EXPECT_EQ( "Hello 22", apply_visitor( f, a, b) );
    EXPECT_EQ( "Hello 44", apply_visitor( f, a, 44 ) );
    
    EXPECT_EQ( "World 22", apply_visitor( f, "World", b ) );
    EXPECT_EQ( "World 44", apply_visitor( f, "World", 44 ) );
}
