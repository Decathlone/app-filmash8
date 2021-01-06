
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