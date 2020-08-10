#include <string>
#include <gtest/gtest.h>

#include "StringHasher.h"
#include "DelphisRound.h"

//------------------------------------------------------------------------------------------
TEST( StringHasher, testCounter ) {
    EXPECT_GE( GetSymbolsCount(), 60U );
    
    size_t lIdx = 0;
