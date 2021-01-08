
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