#include <string>
#include <gtest/gtest.h>

#include "StringHasher.h"
#include "DelphisRound.h"

//------------------------------------------------------------------------------------------
TEST( StringHasher, testCounter ) {
    EXPECT_GE( GetSymbolsCount(), 60U );
    
    size_t lIdx = 0;
    for( size_t i=0; i<100; ++i ){
        EXPECT_NE( GetSymbolByIdx( lIdx++ ) , '_' );
        if( lIdx >= GetSymbolsCount() ) { lIdx = 0; }
    }
    
    EXPECT_EQ( GetSymbolByIdx( GetSymbolsCount() + 1 ) , '_' );
}

//------------------------------------------------------------------------------------------
TEST( StringHasher, testStrToInt ) {
    
    std::string lHash( IntToCode( ToInt( GetSymbols