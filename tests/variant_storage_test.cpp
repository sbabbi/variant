
#include <string>
#include <vector>

#include "variant_storage.hpp"
#include "type_name.hpp"
#include "gtest/gtest.h"

TEST(variant_storage, standard_layout) 
{
    typedef variant_storage<int, char, double> vs_icd;

    EXPECT_TRUE( std::is_literal_type< vs_icd >::value );
    EXPECT_TRUE( (std::is_nothrow_constructible< vs_icd, std::integral_constant<std::size_t, 1>, char >::value) );

    constexpr vs_icd x {std::integral_constant<std::size_t, 0>{}, 5};
    EXPECT_EQ( 5, get<0>(x) );
    
    vs_icd y {std::integral_constant<std::size_t, 1>{},'b'};
    EXPECT_EQ( 'b', get<1>(y) );
    
    y = vs_icd{std::integral_constant<std::size_t, 2>{},3.14};
    EXPECT_EQ( 3.14, get<2>(y) );
}

TEST(variant_storage, unrestricted)
{
    typedef variant_storage<std::string, std::vector<int> > vs_svi;
    
    EXPECT_TRUE( (std::is_nothrow_constructible< vs_svi, std::integral_constant<std::size_t, 0>, std::string&& >::value) );
    
    vs_svi x { std::integral_constant<std::size_t, 0>{}, "Hello, World!" };
    EXPECT_EQ( "Hello, World!", get<0>(x) );
    EXPECT_TRUE( (std::is_rvalue_reference< decltype((get<0>(std::move(x)) ) )>::value ) );
    EXPECT_TRUE( (std::is_lvalue_reference< decltype((get<0>(x) ) )>::value ) );
    
    using std::string;
    get<0>(x).~string();
    
    new (&get<1>(x)) std::vector<int>({5,7,11,32});
    EXPECT_EQ( std::vector<int>({5, 7, 11, 32}), get<1>(x) );
}

template<std::size_t I>
static constexpr auto test_constexpr( variant_storage< int, char > VS )
{
    return get<I>(VS);
};

constexpr variant_storage< int, char > bar{ std::integral_constant<std::size_t, 0>{}, 42 };
static_assert( test_constexpr<0>(bar) == 42, "" );

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
    std::string operator()(T &&) const & noexcept( false )
    { 
        return "refprint(" + type_name<T &&>() + ") const &"; 
    }

    template<class T, class = typename std::enable_if< std::is_rvalue_reference<T&&>::value>::type >
    std::string operator()(T &&) && noexcept( true )
    { 
        return "refprint(" + type_name<T &&>() + ") &&"; 
    }
};

TEST(variant_storage, invoke_variant_storage_ref)
{
    typedef variant_storage< std::string, int> var_t;
    var_t a ( std::integral_constant<std::size_t, 0>{}, "Hello" );
    var_t b ( std::integral_constant<std::size_t, 1>{}, 22 );

    refprint print;
    
    EXPECT_EQ( "refprint(int const &) const &", invoke(print, b, 1) );
    EXPECT_EQ( "refprint(std::string const &) const &", invoke(print, a, 0 ) );
    
    EXPECT_EQ( "refprint(int const &) &&", invoke(refprint{}, b, 1) );
    EXPECT_EQ( "refprint(std::string const &) &&", invoke(refprint{}, a, 0 ) );

    EXPECT_EQ( "refprint(int &&) const &", invoke(print, std::move(b), 1 ) );
    EXPECT_EQ( "refprint(std::string &&) const &", invoke(print, std::move(a), 0 ) );

    EXPECT_EQ( "refprint(int &&) &&", invoke(refprint{}, std::move(b), 1 ) );
    EXPECT_EQ( "refprint(std::string &&) &&", invoke(refprint{}, std::move(a), 0 ) );
}

TEST(variant_storage, invoke_variant_storage_ref_noexcept)
{
    typedef variant_storage< std::string, int> var_t;
    var_t a ( std::integral_constant<std::size_t, 0>{}, "Hello" );

    refprint print;
    
    static_assert(!std::is_nothrow_copy_constructible<std::string>::value,"");
    static_assert(std::is_nothrow_move_constructible<std::string>::value,"");
    static_assert( noexcept(refprint{}), "");

    EXPECT_TRUE( noexcept( print(std::declval<int &>()) ) );
    EXPECT_FALSE( noexcept( print(std::declval<std::string &>()) ) );
    
    EXPECT_FALSE( noexcept(invoke(print, a, 0) ) );
    EXPECT_FALSE( noexcept(invoke(refprint{}, a, 0 ) ) );
    EXPECT_FALSE( noexcept(invoke(print, std::move(a), 0) ) );
    EXPECT_TRUE( noexcept(invoke(refprint{}, std::move(a), 0 ) ) );
}
