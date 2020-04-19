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
        TSimpleTick l