
/** 
 * This is an independent project of an individual developer. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 * \file Indicators.cpp
 * \author kan <kansoftware.ru>
 * \since 2015-11-10
 * \date 2020-10-21
 */

#include <cmath>
#include <stdexcept>

#include "DelphisRound.h"
#include "Comparers.h"
#include "Forecasting.h"
#include "PnlAction.h"
#include "RollRange.h"
#include "RollRangeBar.h"

#include "Indicators.h"

//------------------------------------------------------------------------------------------
TPriceSeries _TrueRange( const TBarSeries & aBars ) {

    TPriceSeries lResult;
    lResult.reserve( aBars.size() );

    if( not aBars.empty() ) {

        TSimpleBar lPriorBar( *aBars.begin() );
        
        lResult.emplace_back( 
            lPriorBar.DateTime,
            lPriorBar.High - lPriorBar.Low,
            lPriorBar.Volume 
        );

        for( size_t i = 1; i < aBars.size(); ++i ) {
            const TSimpleBar lCurrentBar( aBars[ i ] );
            const TPrice lHigh = std::max( lPriorBar.Close , lCurrentBar.High );
            const TPrice lLow = std::min( lPriorBar.Close , lCurrentBar.Low );

            lResult.emplace_back(
                lCurrentBar.DateTime,
                lHigh - lLow,
                lCurrentBar.Volume
            );
            lPriorBar = lCurrentBar;
        }
    }

    return lResult;
}

//------------------------------------------------------------------------------------------
TPriceSeries _AverageTrueRange( const TBarSeries & aBars, const int aPeriod ) {
    const TPriceSeries lPrices( _TrueRange( aBars ) );
    const TPriceSeries lTMPResult( _SmoothedMA( lPrices, aPeriod, 1 ) );
    return lTMPResult;
}

//------------------------------------------------------------------------------------------
TPriceSeries _SimpleMA( const TPriceSeries & aPrices, const int aPeriod, const size_t aLag ) {