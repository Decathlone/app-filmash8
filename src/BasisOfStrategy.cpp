
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
