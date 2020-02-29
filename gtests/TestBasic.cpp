#include <gtest/gtest.h>

#include "Comparers.h"
#include "DelphisRound.h"
#include "Prices.h"

//------------------------------------------------------------------------------------------
TEST( Basic, testCompare ) {
    EXPECT_TRUE( IsGreat( 5.6, 5.5 ) );
    EXPECT_FALSE( IsGreat( 5.5, 5.5 ) );
    EXPECT_FALSE( IsGreat( 5.5, 5.6 ) );
    
    
    EXPECT_FALSE( IsLess( 5.6, 5.5 ) );
    EXPECT_FALSE( IsLess( 5.5, 5.5 ) );
    EXPECT_TRUE( IsLess( 5.5, 5.6 ) );
    
    EXPECT_FALSE( IsNotEqual( 5.5, 5.5 ) );
    EXPECT_TRUE( IsNotEqual( 5.5, 5.6 ) );
    
    EXPECT_FALSE( 