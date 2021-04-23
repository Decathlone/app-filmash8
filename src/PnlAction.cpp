
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/** 
 * File:   PnlAction.cpp
 * Author: kan
 * 
 * Created on 2015.11.11
 * @lastupdate 2018.05.18
 */

#include <map>
#include <list>
#include <vector>
#include <set>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <climits>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <boost/math/distributions/students_t.hpp>

#include "DelphisRound.h"
#include "Comparers.h"

//#define FULLDATA
#ifdef FULLDATA
    #include <iostream>
    #include "Indicators.h"
#endif

#include "PnlAction.h"

//------------------------------------------------------------------------------------------
TPriceSeries DealsToPnLs( const TDeals & aDeals ) {
    
    //<editor-fold desc="Удалить все пустые сделки">
    TDeals lDeals( aDeals );
    auto it = lDeals.begin();
    while( it != lDeals.end() ) {
        if( isZero( it->ClosePrice - it->OpenPrice ) ) {
            it = lDeals.erase( it );
        } else {
            ++it;
        }
    }
    //</editor-fold>
    
    TPriceSeries lPnLs; //( lDeals.size() );
    lPnLs.reserve(lDeals.size());

    //<editor-fold desc="Собственно конвертация">
    for( const TDeal& lDeal : lDeals ) {
        lPnLs.emplace_back(
            lDeal.CloseTime,
            ( (lDeal.DealSide == TDealSide::Buy) ? (lDeal.ClosePrice - lDeal.OpenPrice) : (lDeal.OpenPrice - lDeal.ClosePrice) ),
            ToDouble( lDeal.Volume )
        );
    }
    //</editor-fold>

    return lPnLs;
}

//------------------------------------------------------------------------------------------
bool CalcDrawDown(
    const TPriceSeries & aPnL,
    TPrice & aoMaxDD,
    TInnerDate & aoBegin,
    TInnerDate & aoReturn ) {

    aoBegin = -1;
    aoReturn = -1;

    if( aPnL.empty() ) {
        aoMaxDD = -1;
        return false;
    }

    TPrice lMax = 0.0;
    TPrice lMin = 0.0;
    TPrice lPnl = 0.0;
    TPrice lDelta = 0.0;
    size_t lBegin = 0;
    bool lHaveMin = false;
    bool lResult = false;

    for( size_t i = 0; i < aPnL.size() ; ++i ) {
        lPnl += aPnL[i].Price;

        if( IsGreat( lPnl, lMax ) ) { //очередной максимум
            lMax = lPnl;
            lMin = lPnl;

            if( lHaveMin ) {
                    aoBegin = aPnL[ lBegin ].DateTime;
                    aoReturn = aPnL[ i ].DateTime;
                    lHaveMin = false;
                    lResult = true;
            }

            lBegin = i;

        } else if( IsGreat( lMin, lPnl ) ) { //новый минимум
            lMin = lPnl;

            if( IsGreat( lMax - lMin, lDelta ) ) {
                lDelta = lMax - lMin;
                lHaveMin = true;
            }
        }
    }

    aoMaxDD = lDelta;

    return lResult;
}

//------------------------------------------------------------------------------------------
TPriceSeries ReductionOfTheIncome(
    const TPriceSeries & aPnL,
    const size_t aProfitNum,
    const double aProfitCoef,
    const size_t aLossNum,
    const double aLossCoef ) {

    std::multimap< TPrice, size_t > lProfits;
    for( size_t i = 0; i < aPnL.size(); ++i ) {

        TPrice lValue = aPnL[ i ].Price ;
        //lProfits.insert( std::pair< TPrice, size_t >( lValue, i ) ) ;
        lProfits.emplace( lValue, i ) ;
    }

    TPriceSeries lPnLVector( aPnL );
    size_t i = aProfitNum;
    for( auto it = lProfits.rbegin(); it != lProfits.rend(); ++it ) {
        if( it->first > 0 ) {
            const size_t lRealID = it->second ;
            TSimpleTick lTick = aPnL[ lRealID ] ;
            lTick.Price  /= aProfitCoef ;
            lPnLVector[ lRealID ] = lTick;
        }
        if( --i == 0 ) break;
    }

    i = aLossNum;
    for( auto it = lProfits.begin(); it != lProfits.end() ;++it ) {
        if( it->first < 0 ) {
            const size_t lRealID = it->second ;
            TSimpleTick lTick = aPnL[ lRealID ] ;
            lTick.Price  *= aLossCoef ;
            lPnLVector[ lRealID ] = lTick;
        }
        if( --i == 0 ) break;
    }

    return lPnLVector;
}

//------------------------------------------------------------------------------------------
TPrice PnLsToMoneyResult( const TPriceSeries & aPnls, const bool aUseVolume ) {
    TPrice lResult = 0.0;
    for( const TSimpleTick& lPnl : aPnls ) {
        lResult += lPnl.Price * ( aUseVolume ? lPnl.Volume : 1.0 );
    }
    
    return lResult;
}

//------------------------------------------------------------------------------------------
double Student_t_value( const size_t Sn, const double tail=0.2 ){//80%
    using boost::math::students_t;
    students_t dist( ToDouble(Sn) - 1 );
    const double T = quantile(complement(dist, tail / 2.0));
    
    return T;
}

//------------------------------------------------------------------------------------------
TPrice PnLsToMoneyStatValue( const TPriceSeries & aPnl, const bool aUseVolume, const size_t N, const double aQuantile ) {
    const size_t lSize = aPnl.size();
    if( lSize < 2 or N < 2 ) {
        return 0.0;
    }
    
    std::vector< TPrice > lPnl( lSize );
    TPrice sum = 0.0;
    for( size_t i=0; i<lSize; ++i ){
        const TPrice lValue = aPnl[i].Price * ( aUseVolume ? aPnl[i].Volume : 1.0 );
        sum += lValue;
        lPnl[i] = lValue;
    }
    
    const TPrice mean = sum / ToDouble(lSize);
    
    TPrice lstdev = 0.0;
    for( size_t i=0; i<lSize; ++i ){
        const double ld = lPnl[i] - mean;
        lstdev += ld*ld;
    }
    
    lstdev /= ToDouble(lSize-1);
    
    const double ltStud = Student_t_value( lSize, aQuantile );

    return (mean - ltStud * sqrt(lstdev/ToDouble(N))) * ToDouble(N);
}

//------------------------------------------------------------------------------------------
TPrice PnLsToMoneyStatValueGost( const TPriceSeries & aPnl, const bool aUseVolume, const size_t N ) {
    const size_t lSize = aPnl.size();
    if( lSize < 2 ) {
        return 0.0;
    }
    
    std::vector< TPrice > lPnl( lSize );
    TPrice sum = 0.0;
    for( size_t i=0; i<lSize; ++i ){
        const TPrice lValue = aPnl[i].Price * ( aUseVolume ? aPnl[i].Volume : 1.0 );
        sum += lValue;
        lPnl[i] = lValue;
    }
    
    const TPrice mean = sum / ToDouble(lSize);
    
    TPrice lstdev = 0.0;
    for( size_t i=0; i<lSize; ++i ){
        const double ld = lPnl[i] - mean;
        lstdev += ld*ld;
    }
    
    lstdev /= (ToDouble(lSize) - 1.5);//ГОСТ Р 8.736-2011
    const double ltStud = 2.04 ; //N>30, P=95%
    return ( mean - ltStud * sqrt( lstdev / ToDouble(lSize) ) ) * ToDouble(N);
}

//------------------------------------------------------------------------------------------
TPrice PnLsToMoneyMonteCarlo( const TPriceSeries & aPnl, const bool aUseVolume, const size_t N, const size_t aSamples ){
    const size_t lSize = aPnl.size();
    if( lSize < 2 or N < 2 ) {
        return 0.0;
    }
    
    TPrice lResult = ToDouble( ULLONG_MAX ); //очень большое число
    
    for( size_t i=0; i<aSamples; ++i ){
        TPrice lPnlTest = 0.0;
        for( size_t j=0; j<N; ++j ){
            const size_t lID = rand() % lSize;
            const TPrice lPnlSample = aPnl[lID].Price * (aUseVolume ? aPnl[lID].Volume : 1.0);
            lPnlTest += lPnlSample;
        }
        lPnlTest /= ToDouble(N);
        
        lResult = std::min(lResult, lPnlTest);
    }
    
    return lResult * ToDouble(N);
}

//------------------------------------------------------------------------------------------