
#include <string>
#include <vector>

#include "variant.hpp"
#include "type_name.hpp"
#include "gtest/gtest.h"

TEST(variant, deduce_overload) 
{
    typedef variant<int, std::string, double> v_isd;
    
    EXPECT_EQ( 0, v_isd::deduce_overload_t<int&>::value );
    EXPECT_EQ( 1, v_isd::deduce_overload_t<const char *>::value );
    EXPECT_EQ( 2, v_isd::deduce_overload_t<double&>::value );
    
    typedef variant<std::size_t, std::vector<int>> v_iv;
    EXPECT_EQ( 0, v_iv::deduce_overload_t<std::size_t>::value );
}

TEST(variant, default_construct)
{
    typedef variant<std::string, std::vector<int> > vs_svi;
    
    EXPECT_EQ(0, vs_svi().which() );
}

TEST(variant, overload_constructor)
{
    typedef variant<std::size_t, std::vector<int> > v1_t;
    typedef variant<std::size_t, char, std::vector<int> > v2_t;
    
    v2_t x = v1_t(2);
    EXPECT_EQ(0, x.which() );
    
    x = v1_t{1,2,3,4};
    EXPECT_EQ(2, x.which() );
    EXPECT_EQ( (std::vector<int>{1,2,3,4}), get<std::vector<int>>(x) );
    
    x = 'b';
    EXPECT_EQ(1, x.which() );
}

TEST(variant, init_list_constructor)
{
    variant<std::vector<int>, std::string, int> f = {2,4,11,32};
    
    EXPECT_EQ(0, f.which());
}
