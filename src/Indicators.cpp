
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

        for( size_t i =  lPeriod+aLag ; i < lResultSize; ++i ) {
            lExpPrice = aPrices[ i ].Price * lExpWeigth + lExpPrice * ( 1 - lExpWeigth );
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, lExpPrice, 1.0 };
        }

    } else {
        for( size_t i = 0 ; i < lResultSize; ++i ) {
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, GetBadPrice(), 0.0 };
        }
    }

    return lResult;
}

//------------------------------------------------------------------------------------------
TPriceSeries _SmoothedMA( const TPriceSeries & aPrices, const int aPeriod, const size_t aLag ) {

    if( aPeriod <= 0 ) {
        throw std::logic_error( "Period can be only positive!" );
    }
    const size_t lPeriod = static_cast< size_t >( aPeriod );
    const size_t lResultSize = aPrices.size() ;
    TPriceSeries lResult( lResultSize );

    if( lResultSize > lPeriod + aLag ) {

        double lExpPrice = 0.0 ;

        for( size_t i = 0 ; i < aLag; ++i ) {
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, GetBadPrice(), 0.0 };
        }

        for( size_t i = aLag ; i < lPeriod + aLag ; ++i ) {
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, GetBadPrice(), 0.0 };
            lExpPrice += aPrices[ i ].Price;
        }
        lExpPrice /= ToDouble( lPeriod );
        lResult[ lPeriod + aLag - 1 ] = TSimpleTick{ aPrices[ lPeriod + aLag - 1 ].DateTime, lExpPrice, 1.0 } ;

        for( size_t i =  lPeriod + aLag ; i < lResultSize; ++i ) {
            lExpPrice = ( aPrices[ i ].Price + lExpPrice * ToDouble( lPeriod - 1 ) ) / ToDouble( lPeriod );
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, lExpPrice, 1.0 };
        }

    } else {
        for( size_t i = 0 ; i < lResultSize; ++i ) {
            lResult[ i ] = TSimpleTick { aPrices[ i ].DateTime, GetBadPrice(), 0.0 };
        }
    }

    return lResult;
}

//------------------------------------------------------------------------------------------
bool _DM( const TBarSeries & aBars, TPriceSeries & aoDMp, TPriceSeries & aoDMn ) {
    size_t lBarsCounter = aBars.size() ;
    if( lBarsCounter <= 1 ) { return false; }

    TPriceSeries lDMp( lBarsCounter );
    TPriceSeries lDMn( lBarsCounter );

    TSimpleTick lTick {
        aBars.begin()->DateTime,
        0.0,
        aBars.begin()->Volume 
    };
    lDMp[ 0 ] = lTick;
    lDMn[ 0 ] = lTick;

    for( size_t i = 1; i < lBarsCounter; ++i ) {
        TSimpleBar lCurrentBar = aBars[ i ];
        
        const TPrice lHigh = lCurrentBar.High - aBars[ i-1 ].High;
        const TPrice lLow = aBars[ i-1 ].Low - lCurrentBar.Low;

        lTick.DateTime = lCurrentBar.DateTime ;
        lTick.Volume = lCurrentBar.Volume ;

        TPrice dm_p = 0.0;
        TPrice dm_m = 0.0;

        if( IsGreat( lHigh, lLow ) and isPositiveValue( lHigh ) ) {
            dm_p = lHigh;
        }

        else if( IsGreat( lLow, lHigh ) and isPositiveValue( lLow ) ) {
            dm_m = lLow;
        }

        lTick.Price = dm_p ;
        lDMp[ i ] = lTick;

        lTick.Price = dm_m ;
        lDMn[ i ] = lTick;
    }

    aoDMp.swap( lDMp );
    aoDMn.swap( lDMn );
    return true;
}

//------------------------------------------------------------------------------------------
bool _DI( const TBarSeries &aBars, const int aPeriod, TPriceSeries &aoDMIp, TPriceSeries &aoDMIn ) {
    if( aPeriod < 2 ) { return false; }
    
    const size_t lPeriod = ToSize_t( aPeriod );
    
    if( aBars.size() < lPeriod ) { return false; }

    TPriceSeries lDMp;
    TPriceSeries lDMn;

    if( not _DM( aBars, lDMp, lDMn ) ) { return false; }

    const TPriceSeries lTR( _TrueRange( aBars ) );

    if( aBars.size() != lDMp.size() or lDMp.size() != lDMn.size() or lDMn.size() != lTR.size() ) { return false; }

    aoDMIp.resize( aBars.size() );
    aoDMIn.resize( aBars.size() );

    double lDMSump = 0.0;
    double lDMSumn = 0.0;
    double lTRSum = 0.0;
    for( size_t i = 0; i < lPeriod; ++i ) {
        const TSimpleTick lTick{ aBars[i].DateTime, GetBadPrice(), 0.0 };

        aoDMIp[i] = lTick;
        aoDMIn[i] = lTick;

        lDMSump += lDMp[i].Price;
        lDMSumn += lDMn[i].Price;
        lTRSum += lTR[i].Price;
    }

    for( size_t i = lPeriod; i < aBars.size(); ++i ) {