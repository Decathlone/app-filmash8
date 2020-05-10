#include <gtest/gtest.h>

#include "BasisOfStrategy.h"
#include "PnlAction.h"
#include "DelphisRound.h"
#include "VolatilityBuffer.h"
#include "VolatilityBarBuffer.h"
#include "RubberBuffer.h"

//------------------------------------------------------------------------------------------
TPriceSeries GetSimplePnl(){
    TPriceSeries lPnLs( 10 );

    for( size_t i=0; i<lPnLs.size(); ++i ) {
        TSimpleTick lTick {
            gStartingTime + ToDouble(i)*gOneDay,
            1.0,
            2.0
        };

        lPnLs[ i ] = lTick;
    }
    
    return lPnLs;
}

//------------------------------------------------------------------------------------------
TEST( PnLs, PnLsToMoneyResult ) {

    const TPriceSeries lPnLs( GetSimplePnl() );
    
    EXPECT_EQ( PnLsToMoneyResult( lPnLs, false ), 10.0 );
    EXPECT_EQ( PnLsToMoneyResult( lPnLs, true ), 20.0 );
}

//------------------------------------------------------------------------------------------
TEST( PnLs, PnlsToDaily ) {

    const TPriceSeries lPnLs( GetSimplePnl() );
    const TPriceSeries lPnls_Daily( PnlsToDaily(lPnLs) );
    
    EXPECT_EQ( lPnLs.size(), lPnls_Daily.size() );
    
    double lCumPnl = 0.0;
    for( size_t i = 0; i<lPnLs.size(); ++i ){
        lCumPnl += lPnLs[i].Price*lPnLs[i].Volume;
        EXPECT_EQ( lCumPnl, lPnls_Daily[i].Price );
        EXPECT_EQ( lPnls_Daily[i].Volume, 1.0 );
    }
}

//------------------------------------------------------------------------------------------
TEST( PnLs, PnLsAmplifier 