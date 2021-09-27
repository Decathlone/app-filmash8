// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <vector>
#include <list>
#include <map>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>

#include "DelphisRound.h"

#include "RExportFile.h"

//------------------------------------------------------------------------------------------
Rcpp::NumericVector SimpleMA( const Rcpp::NumericMatrix & aXts, const int aPeriod, const int aType ) {
    const TMAPoint lMAPoint = static_cast< TMAPoint >( aType );
    std::string lTZone;
    const TPriceSeries lPrices( XtsToPriceSeries( aXts, lMAPoint, lTZone ) );
    const TPriceSeries lTMPResult( _SimpleMA( lPrices, aPeriod ) );

    return PriceSeriesToXts( lTMPResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector ExponentMA( const Rcpp::NumericMatrix & aXts, const int aPeriod, const int aType ) {
    const TMAPoint lMAPoint = static_cast< TMAPoint >( aType );
    std::string lTZone;
    const TPriceSeries lPrices( XtsToPriceSeries( aXts, lMAPoint, lTZone ) );
    const TPriceSeries lTMPResult( _ExponentMA( lPrices, aPeriod ) );

    return PriceSeriesToXts( lTMPResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector KaufmanMA( const Rcpp::NumericMatrix & aXts, const int aPeriod, const double aKoeff, const int aType ) {
    const TMAPoint lMAPoint = static_cast< TMAPoint >( aType );    
    std::string lTZone;    
    const TPriceSeries lPrices( XtsToPriceSeries( aXts, lMAPoint, lTZone ) );    
    const TPriceSeries lTMPResult( _KAMA( lPrices, aPeriod, aKoeff ) );    
    return PriceSeriesToXts( lTMPResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector TrueRange( const Rcpp::NumericMatrix & aXts ) {
    const TBarSeries lBars( XtsToBarSeries( aXts ) );
    const TPriceSeries lResult( _TrueRange( lBars ) );
    const std::string lTZone( Rcpp::as< std::string >( aXts.attr( "tzone" ) ) );
    
    return PriceSeriesToXts( lResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector AverageTrueRange( const Rcpp::NumericMatrix & aXts, const int aPeriod ) {
    const