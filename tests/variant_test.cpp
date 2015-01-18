
#include <string>
#include <vector>

#include "variant.hpp"
#include "type_name.hpp"
#include "gtest/gtest.h"

TEST(variant, deduce_overload) 
{
    typedef variant<int, std::string, double> v_isd;
    
    EXPECT_EQ( 0, v_isd::deduce_overload<int&>::value );
    EXPECT_EQ( 1, v_isd::deduce_overload<const char *>::value );
    EXPECT_EQ( 2, v_isd::deduce_overload<double&>::value );
}

TEST(variant, default_construct)
{
    typedef variant<std::string, std::vector<int> > vs_svi;
    
    EXPECT_EQ(0, vs_svi().which() );
}

