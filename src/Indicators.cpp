
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

    if( aPeriod <= 0 ) {
        throw std::logic_error( "Period can be only positive!" );
    }
    
    const size_t lPeriod = ToSize_t( aPeriod );
    const size_t lResultSize = aPrices.size() ;
    
    TPriceSeries lResult;
    lResult.reserve( lResultSize );
    
    if( lResultSize > lPeriod ) {
        
        for( size_t j = 0; j < aLag ; ++j ) {
            lResult.emplace_back( aPrices[ j ].DateTime, GetBadPrice(), 0.0 );
        }
        
        TPrice lSum = 0.0;
        for( size_t j = aLag; j < lPeriod - 1 ; ++j ) {
            lResult.emplace_back( aPrices[ j ].DateTime, GetBadPrice(), 0.0 );
            lSum += aPrices[ j ].Price;
        }

        for( size_t i = lPeriod - 1; i < lResultSize; ++i ) {
            lSum += aPrices[ i ].Price;
            lResult.emplace_back( aPrices[ i ].DateTime, lSum / ToDouble( lPeriod ), 1.0 );
            lSum -= aPrices[ i - lPeriod + 1 ].Price;
        }

    } else {
        for( size_t i = 0; i < lResultSize; ++i ) {
            lResult.emplace_back( aPrices[ i ].DateTime, GetBadPrice(), 0.0 );
        }
    }

    return lResult;
}

//------------------------------------------------------------------------------------------
TPriceSeries _ExponentMA( const TPriceSeries & aPrices, const int aPeriod, const size_t aLag ) {

    if( aPeriod <= 0 ) {
        throw std::logic_error( "Period can be only positive!" );
    }
    const size_t lPeriod = static_cast< size_t >( aPeriod );
    const size_t lResultSize = aPrices.size() ;
    TPriceSeries lResult( lResultSize );

    if( lResultSize > lPeriod ) {
      
        for( size_t i = 0 ; i < aLag; ++i ) {
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, GetBadPrice(), 0.0 };
        }

        const double lExpWeigth = 2.0 / ( ToDouble( lPeriod + 1 ) );
        double lExpPrice = 0.0 ;
        
        for( size_t i = aLag ; i < lPeriod + aLag; ++i ) {
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, GetBadPrice(), 0.0 };
            lExpPrice += aPrices[ i ].Price;
        }
        lExpPrice /= ToDouble( lPeriod );
        lResult[ lPeriod + aLag - 1 ] = TSimpleTick{ aPrices[ lPeriod + aLag - 1 ].DateTime, lExpPrice, 1.0 } ;
