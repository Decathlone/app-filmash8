
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
        lDMSump = ( lDMSump * ToDouble( lPeriod - 1 ) / ToDouble( lPeriod ) + lDMp[i].Price );
        lTRSum = ( lTRSum * ToDouble( lPeriod - 1 ) / ToDouble( lPeriod ) + lTR[i].Price );
        const double lDMIp = 
            ( isPositiveValue( lTRSum ) ?
                (100.0 * lDMSump / lTRSum) :
                0.0 );
        TSimpleTick lTick{ aBars[i].DateTime, lDMIp, aBars[i].Volume };
        aoDMIp[i] = lTick;

        lDMSumn = ( lDMSumn * ToDouble( lPeriod - 1 ) / ToDouble( lPeriod ) + lDMn[i].Price );
        const double lDMIn = 
            ( isPositiveValue( lTRSum ) ?
                (100.0 * lDMSumn / lTRSum) :
                0.0 );
        lTick.Price = lDMIn;
        aoDMIn[i] = lTick;
    }

    return true;
}

//------------------------------------------------------------------------------------------
TPriceSeries _ADX( const TBarSeries & aBars, const int aPeriod ) {
    TPriceSeries lDMIp;
    TPriceSeries lDMIn;
    if( not _DI( aBars, aPeriod, lDMIp, lDMIn )) {
        return TPriceSeries();
    }
    
    TPriceSeries lDX( aBars.size() );
    for( size_t i = 0; i < ToSize_t(aPeriod); ++i ){
        lDX[i].DateTime = aBars[i].DateTime;
        lDX[i].Price = GetBadPrice();
        lDX[i].Volume = 0.0;
    }
    
    // DX = [ 100 * ABS( (+DI) - (-DI) ) ] / ( (+DI) + (-DI) )
    for( size_t i = ToSize_t(aPeriod); i < aBars.size(); ++i ){
        lDX[i].DateTime = aBars[i].DateTime;
        lDX[i].Volume = 1.0;
        
        lDX[i].Price = std::fabs( 100.0 * ( lDMIp[i].Price - lDMIn[i].Price ) ) / ( lDMIp[i].Price + lDMIn[i].Price );
    }
    
     //ADXi = [(ADX(i-1) * (n - 1)) + DXi] / n where n = Smoothing Period
    const TPriceSeries lADX( _SmoothedMA( lDX, aPeriod, aPeriod ));
    
    return lADX;
}

//------------------------------------------------------------------------------------------
TPriceSeries _ParabolicSar( const TBarSeries & aBars, const double aAf, const double aMaxAf ) {

    TPriceSeries lResult( aBars.size() );

    if( aBars.empty() ) {
        return lResult;
    }

    TDealSide lCurrentSide = TDealSide::Buy;
    const double lInitaAfValue = aAf;
    double lAf = lInitaAfValue;

    const TSimpleBar lFirstBar( *aBars.begin() );

    const TSimpleTick lFirstTick{
        lFirstBar.DateTime,
        lFirstBar.Low,
        1.0
    };
    lResult[ 0 ] = lFirstTick;

    double lSar = lFirstBar.Low;
    double lFuturesSar = lFirstBar.High;

    for( size_t i = 1; i < aBars.size(); ++i ) {
        const TSimpleBar lCurrentBar( aBars[ i ] );
        const TPrice lHigh = lCurrentBar.High;
        const TPrice lLow = lCurrentBar.Low;
        const double lPriorSar = lSar;

        lSar = (lFuturesSar - lPriorSar) * lAf + lPriorSar;
        
        if( lCurrentSide == TDealSide::Buy ) {

            if( IsGreat( lHigh, lFuturesSar ) ) {
                lFuturesSar = lHigh;
                if( IsLess(lAf, aMaxAf) ) {
                    lAf += aAf;
                }
            }
                
            const TPrice lPriorLow = aBars[ i - 1 ].Low;
            const TPrice lLocalLow = std::min( lLow, lPriorLow );
            const double lCondidateSAR = std::min( lLocalLow, lSar );
            lSar = std::max( lPriorSar, lCondidateSAR );
            
            if( IsLess( lLow , lSar ) ) {
                lAf = lInitaAfValue;
                lCurrentSide = TDealSide::Sell;
                lSar = lFuturesSar;
                lFuturesSar = lLow;
            }
                
            const TSimpleTick lTick{
                lCurrentBar.DateTime,
                lSar,
                1.0
            };
            lResult[ i ] = lTick;
            continue;
        }

        if( lCurrentSide == TDealSide::Sell ) {
                
            if( IsLess(lLow, lFuturesSar) ) {
                lFuturesSar = lLow;
                if( IsLess( lAf, aMaxAf) ) {
                    lAf += aAf;
                }
            }

            const TPrice lPriorHigh = aBars[ i - 1 ].High;
            const TPrice lLocalHigh= std::max( lHigh, lPriorHigh );
            const double lCondidateSAR = std::max( lLocalHigh, lSar );
            lSar = std::min( lPriorSar, lCondidateSAR );
            
            if( IsGreat(lHigh, lSar) ) {
                lAf = lInitaAfValue;
                lCurrentSide = TDealSide::Buy;
                lSar = lFuturesSar;
                lFuturesSar = lHigh;
            }

            const TSimpleTick lTick{
                lCurrentBar.DateTime,
                lSar,
                1.0
            };
            lResult[ i ] = lTick;
//            continue;
        }
    }
    
    return lResult;
}

//------------------------------------------------------------------------------------------
TPriceSeries _MACD( 
    const TPriceSeries & aPrices, 
    const int aFastPeriod, 
    const int aSlowPeriod, 
    const int aSmoothPeriod ) {
    
    if( aFastPeriod <= 0 or aSlowPeriod <= 0 or aSmoothPeriod<=0 ) {
        throw std::logic_error( "Period can be only positive!" );
    }
    
    const TPriceSeries lFastExponentMA( _ExponentMA( aPrices, aFastPeriod ) );
    const TPriceSeries lSlowExponentMA( _ExponentMA( aPrices, aSlowPeriod ) );
    
    const size_t lResultSize = aPrices.size() ;
    TPriceSeries lDifference( lResultSize );
    
    const size_t lMaxPeriod = ToSize_t( ( aSlowPeriod > aFastPeriod ) ? aSlowPeriod : aFastPeriod );
    
    for( size_t j = 0; j < lMaxPeriod-1 ; ++j ) {
        lDifference[ j ] = TSimpleTick { aPrices[ j ].DateTime, GetBadPrice(), 0.0 };
    }
    
    for( size_t i = lMaxPeriod-1 ; i < lResultSize; ++i ) {
        lDifference[ i ] = TSimpleTick { aPrices[ i ].DateTime, ( lFastExponentMA[ i ].Price - lSlowExponentMA[ i ].Price ), 1.0 };
    }
    
    const TPriceSeries lResult( _ExponentMA( lDifference, aSmoothPeriod, lMaxPeriod-1 ) );
    
    return lResult;
}

//------------------------------------------------------------------------------------------
TPriceSeries _ZigZag( const TBarSeries & aBars, const double aGap ) {
    
    if( not isPositiveValue( aGap )  ) {
        throw std::logic_error( "aGap can be only positive!" );
    }
    
    TPriceSeries lResult( aBars.size() );
    
    /////////////////////////////