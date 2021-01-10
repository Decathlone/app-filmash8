
//
// Created by kan on 05.11.2015.
//

#ifndef BACKTESTER_REXPORTFILE_H
#define BACKTESTER_REXPORTFILE_H

#include <typeinfo>
#include <typeindex>
#include <any>

#include <Rcpp.h>

#include "PointerValidater.h"
#include "FileManipulator.h"
#include "BasisOfStrategy.h"
#include "Indicators.h"
#include "PnlAction.h"
#include "Comparers.h"

// [[Rcpp::export]]
Rcpp::NumericVector SimpleMA( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const int aType = 3 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector ExponentMA( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const int aType = 3 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector KaufmanMA( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const double aKoeff, const int aType = 3 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector TrueRange( const Rcpp::NumericMatrix & aOHLCV );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector AverageTrueRange( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::List DI( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod = 14 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector ZigZag( const Rcpp::NumericMatrix & aOHLCV, const double aGap );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector AbsoluteZigZag( const Rcpp::NumericMatrix & aOHLCV, const double aGap ); 
//------------------------------------------------------------------------------------------

//' @title The Parabolic Stop-and-Reverse calculates a trailing stop. Developed by J. Welles Wilder
//' @name SAR
//' @param aOHLCV xts object contains OHLCV prices
//' @param aAccFactor Acceleration factor
//' @param aMaxAccFactor Maximum acceleration factor
//' @family indicator
//' @return Returns A object containing the Parabolic Stop and Reverse values as xts.
//' @description First value is first bars low.
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector SAR( const Rcpp::NumericMatrix & aOHLCV, const double aAccFactor = 0.02, const double aMaxAccFactor = 0.2, const bool aOvernight = true );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector MACD( const Rcpp::NumericMatrix & aOHLCV, const int aFastPeriod, const int aSlowPeriod, const int aSmoothPeriod, const int aType = 3 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::List Forecasting( const Rcpp::NumericMatrix & aOHLCV, const int aForecastPeriod = 100, const double aConfidenceIntervals = 1.29, const int aType = 3 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::List RollMinMax( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const bool aTouch = false );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::List RollMinMax_old( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const bool aTouch = false );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::List SupportRessistance( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const size_t aMinTouch, const double aTollerance );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::List ForwardMinMax( const Rcpp::NumericMatrix & aOHLCV, const int aTimeDelta );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector ChannelSize( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericMatrix ConvertBars( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod = -1 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector Stochastic( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericVector ADX( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::NumericMatrix BollingerBands( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const double aSigma=2.0, const int aType=6 );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
bool SaveData( const Rcpp::NumericMatrix & aOHLCV, const SEXP & aFileName );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
double DealsToPnLValue( const Rcpp::DataFrame & aDeals, const SEXP & aParams );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
double DealsToStatValue( const Rcpp::DataFrame & aDeals, const SEXP & aParams );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
double DealsToMonteCarloValue( const Rcpp::DataFrame & aDeals, const SEXP & aParams );
//------------------------------------------------------------------------------------------

// [[Rcpp::export]]
Rcpp::List DealsToCoeffUnrealized( const Rcpp::NumericMatrix & aBars, const Rcpp::DataFrame & aDeals, const SEXP & aParams );
//------------------------------------------------------------------------------------------

inline std::string getTimeZone(const Rcpp::NumericMatrix & aData){
    std::string lTZone;
    try{
        lTZone = Rcpp::as< std::string >( aData.attr("tzone") );
    }catch(...){
        //std::cout << "tzone import error"  << std::endl;
    };
    
    return lTZone;
}
//------------------------------------------------------------------------------------------

inline TPriceSeries XtsToPriceSeries( const Rcpp::NumericMatrix & aData, const TMAPoint aType, std::string & aoTZone ) {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wconversion"
    const Rcpp::NumericVector lIndex( aData.attr("index") );
    #pragma GCC diagnostic pop

    aoTZone = getTimeZone( aData );

    if( aData.ncol() < 5 ) {
        throw std::logic_error( "Need Xts with OHLCV bars" );
    }

    TPriceSeries lResult;
    lResult.reserve( aData.nrow() );

    if( aType == TMAPoint::MAP_Mid ) {
        for( int lRowNum = 0; lRowNum < aData.nrow(); ++lRowNum ) {
            const TPrice lPrice = (
                aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_High ) ) +
                aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_Low ) )
            ) / 2.0 ;
            lResult.emplace_back( lIndex[ lRowNum ], lPrice, aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_Volume ) ) );
        }

    } else if( aType == TMAPoint::MAP_Triple ) {
        for( int lRowNum = 0; lRowNum < aData.nrow(); ++lRowNum ) {
            const TPrice lPrice =(
                aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_High ) ) +
                aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_Low ) ) +
                aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_Close ) )
            ) / 3.0 ;
            lResult.emplace_back( lIndex[ lRowNum ], lPrice, aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_Volume ) ) );
        }

    } else {
        for( int lRowNum = 0; lRowNum < aData.nrow(); ++lRowNum ) {
            const TPrice lPrice = aData.at( lRowNum, static_cast<int>( aType ) );
            lResult.emplace_back( lIndex[ lRowNum ], lPrice, aData.at( lRowNum, static_cast<int>( TMAPoint::MAP_Volume ) ) );
        }
    }

    return lResult;
}
//------------------------------------------------------------------------------------------

inline Rcpp::NumericVector PriceSeriesToXts( const TPriceSeries & aPrices, const std::string & aTZone ) {

    if(aPrices.empty()){
        return Rcpp::NumericVector();
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
    Rcpp::NumericVector lResult( aPrices.size() );
    Rcpp::NumericVector lIndex( aPrices.size() );
#pragma GCC diagnostic pop

    for( size_t i=0; i < aPrices.size(); ++i ) {
        lResult[ i ] = IsEqual( aPrices[i].Price, GetBadPrice() ) ? NA_REAL : aPrices[i].Price ;
        lIndex[ i ] = aPrices[i].DateTime;
    }
    
    lIndex.attr("tzone")    = aTZone.c_str();// "Europe/Moscow"; // the index has attributes
    lIndex.attr("tclass")   = "POSIXct";

    lResult.attr("dim")         = Rcpp::IntegerVector::create( aPrices.size(), 1 );
    lResult.attr("index")       = lIndex;
    Rcpp::CharacterVector klass = Rcpp::CharacterVector::create( "xts", "zoo" );
    lResult.attr("class")       = klass;
    lResult.attr(".indexCLASS") = "POSIXct";
    lResult.attr("tclass")      = "POSIXct";
    lResult.attr(".indexTZ")    = aTZone.c_str(); //"Europe/Moscow";
    lResult.attr("tzone")       = aTZone.c_str(); //"Europe/Moscow";

    colnames( lResult ) = Rcpp::CharacterVector::create( "value" );
    
    return lResult;
}
//------------------------------------------------------------------------------------------

inline Rcpp::NumericVector PriceSeriesVolumeToXts( const TPriceSeries & aPrices, const std::string & aTZone ) {
#pragma GCC diagnostic push