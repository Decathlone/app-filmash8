
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