
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/** 
 * File:   BarPeriod.cpp
 * Author: kan
 * 
 * Created on 11.11.2015.
 */

#include <cmath>
#include <time.h>
#include <stddef.h>
#include <cassert>
#include <sstream>

#include "Comparers.h"
#include "DelphisRound.h"
#include "BarPeriod.h"

#include "BasisOfStrategy.h"

//------------------------------------------------------------------------------------------
const std::string whitespaces (" \t\f\v\n\r");
std::string trim( const std::string& str ) {
    const size_t first = str.find_first_not_of( whitespaces );
    if( std::string::npos == first ) {
        return str;
    }
    const size_t last = str.find_last_not_of( whitespaces );
    return str.substr(first, (last - first + 1));
}

//------------------------------------------------------------------------------------------
bool IsOneDay( const TInnerDate aLeft, const TInnerDate aRight) {
    return Trunc( aLeft / gOneDay ) == Trunc( aRight / gOneDay );
}

//------------------------------------------------------------------------------------------
bool IsOneHour( const TInnerDate aLeft, const TInnerDate aRight, const double aDuration ) {
    assert( isPositiveValue( aDuration ) );
    //return IsLess( std::fabs( aLeft - aRight ), gOneHour*aDuration + 1.0 );

    return Trunc( aLeft / (gOneHour*aDuration) ) == Trunc( aRight / (gOneHour*aDuration) );
}

//------------------------------------------------------------------------------------------
bool IsValidBar( const TSimpleBar & aBar ) {
    return IsGreat( aBar.Volume, -2.0 * gAbsoluteZero ) // >= 0
           and IsValidPrice( aBar.Open )
           and IsValidPrice( aBar.High )
           and IsValidPrice( aBar.Low )
           and IsValidPrice( aBar.Close );
}

//------------------------------------------------------------------------------------------
#define FAST_DECODER( aField ) \
    for( std::size_t lRowNum = 0; lRowNum < aBar.size(); ++lRowNum ) { \
        TSimpleBar lBar = aBar[ lRowNum ]; \
        TPrice lPrice = lBar.aField; \
        TSimpleTick lTick { lBar.DateTime, lPrice, lBar.Volume }; \
        lResult[ lRowNum ] = lTick; \
    } \
// end of FAST_DECODER

TPriceSeries BarsToPriceSeries( const TBarSeries &aBar, const TMAPoint aType ) {

    TPriceSeries lResult( aBar.size() );

    switch ( aType ) {
        /////////
        case TMAPoint::MAP_Close : {
            FAST_DECODER( Close );
        } break ;
        /////////
        case TMAPoint::MAP_Mid : {
            for( std::size_t lRowNum = 0; lRowNum < aBar.size(); ++lRowNum ) {
                TSimpleBar lBar = aBar[ lRowNum ];
                TPrice lPrice = ( lBar.High + lBar.Low ) / 2.0 ;
                TSimpleTick lTick { lBar.DateTime, lPrice, lBar.Volume };
                lResult[ lRowNum ] = lTick;
            }
        } break ;
        /////////
        case TMAPoint::MAP_Triple : {
            for( std::size_t lRowNum = 0; lRowNum < aBar.size(); ++lRowNum ) {
                TSimpleBar lBar = aBar[ lRowNum ];
                TPrice lPrice = ( lBar.High + lBar.Low + lBar.Close ) / 3.0 ;
                TSimpleTick lTick { lBar.DateTime, lPrice, lBar.Volume };
                lResult[ lRowNum ] = lTick;
            }
        } break ;
        /////////
        case TMAPoint::MAP_Open : {
            FAST_DECODER( Open );
        } break ;
        /////////
        case TMAPoint::MAP_High : {
            FAST_DECODER( High );
        } break ;
        /////////
        case TMAPoint::MAP_Low : {
            FAST_DECODER( Low );
        } break ;
        /////////