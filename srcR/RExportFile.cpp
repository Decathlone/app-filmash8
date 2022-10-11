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
    const TBarSeries lBars( XtsToBarSeries( aXts ) );
    const TPriceSeries lTMPResult( _AverageTrueRange( lBars, aPeriod ) );
    const std::string lTZone( Rcpp::as< std::string >( aXts.attr( "tzone" ) ) );
    
    return PriceSeriesToXts( lTMPResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector MACD( const Rcpp::NumericMatrix & aXts, const int aFastPeriod, const int aSlowPeriod, const int aSmoothPeriod, const int aType ) {

    if( aFastPeriod <= 0 or aSlowPeriod <= 0 or aSmoothPeriod<=0 ) {
        return R_NilValue; 
    }
    
    const TMAPoint lMAPoint = static_cast< TMAPoint >( aType );

    std::string lTZone;
    const TPriceSeries lPrices( XtsToPriceSeries( aXts, lMAPoint, lTZone ) );
    
    const TPriceSeries lMacdResult( _MACD( lPrices, aFastPeriod, aSlowPeriod, aSmoothPeriod ) );
    const Rcpp::NumericVector oResult( PriceSeriesToXts( lMacdResult, lTZone ) );
    colnames( oResult ) = Rcpp::CharacterVector::create( "Indicator" );
    return oResult;
}

//------------------------------------------------------------------------------------------
Rcpp::List Forecasting( const Rcpp::NumericMatrix & aXts, const int aForecastPeriod, const double aConfidenceIntervals, const int aType ) {
    if( not isPositiveValue( aConfidenceIntervals ) or aForecastPeriod <= 0 ) {
        return R_NilValue; 
    }
    
    const TMAPoint lMAPoint = static_cast< TMAPoint >( aType );
    const size_t lForecastPeriod = aForecastPeriod;

    std::string lTZone;
    const TPriceSeries lPrices( XtsToPriceSeries( aXts, lMAPoint, lTZone ) );
    
    if( lPrices.size() <= lForecastPeriod ) {
        return R_NilValue;
    }
    
    TPriceSeries lUpperBorder;
    TPriceSeries lForecast;
    TPriceSeries lLowerBorder;
    
    if( _Forecasting( lPrices, lForecastPeriod, aConfidenceIntervals, lUpperBorder, lForecast, lLowerBorder ) ) {
    
        ///\todo заменить плохие значения на NA
        return Rcpp::List::create(
            Rcpp::Named("UpperBorder") = PriceSeriesToXts( lUpperBorder, lTZone ),
            Rcpp::Named("Forecast") = PriceSeriesToXts( lForecast, lTZone ),
            Rcpp::Named("LowerBorder") = PriceSeriesToXts( lLowerBorder, lTZone )
        );
        
    } else {
        return R_NilValue;
    }
}

//------------------------------------------------------------------------------------------
double DealsToPnLValue( const Rcpp::DataFrame & aDeals, const SEXP & aParams ) {
    const Rcpp::List lParam( aParams );
    double lFirstPrice = Rcpp::as<double>( lParam["FirstPrice"] );
    double lMinDeals = Rcpp::as<double>( lParam["MinDeals"] );

    const TDeals lDeals( DataFrameToDeals( aDeals, lFirstPrice ) );

    return DealsToPNLCoefficientQuick( lDeals, lFirstPrice, ToSize_t(lMinDeals) );
}

//------------------------------------------------------------------------------------------
double DealsToStatValue( const Rcpp::DataFrame & aDeals, const SEXP & aParams ) {
    const Rcpp::List lParam( aParams );
    double lFirstPrice = Rcpp::as<double>( lParam["FirstPrice"] );
    const size_t N = Rcpp::as<size_t>( lParam["N"] ) ;
    double lQuantile = Rcpp::as<double>( lParam["Quantile"] );

    const TDeals lDeals( DataFrameToDeals( aDeals, lFirstPrice ) );
    const TPriceSeries lPns(DealsToPnLs( lDeals ));
    
    return PnLsToMoneyStatValue( lPns, false, N, lQuantile );
}

//------------------------------------------------------------------------------------------
double DealsToMonteCarloValue( const Rcpp::DataFrame & aDeals, const SEXP & aParams ) {
    const Rcpp::List lParam( aParams );
    double lFirstPrice = Rcpp::as<double>( lParam["FirstPrice"] );
    const size_t N = Rcpp::as<size_t>( lParam["N"] ) ;
    const size_t aSamples = Rcpp::as<size_t>( lParam["Samples"] ) ;
    double lQuantile = Rcpp::as<double>( lParam["Quantile"] );
    
    const TDeals lDeals( DataFrameToDeals( aDeals, lFirstPrice ) );
    const TPriceSeries lPns(DealsToPnLs( lDeals ));
    
    return PnLsToMoneyMonteCarloQuantile( lPns, false, N, aSamples, lQuantile );    
}

//------------------------------------------------------------------------------------------
Rcpp::List DealsToCoeffUnrealized( const Rcpp::NumericMatrix & aBars, const Rcpp::DataFrame & aDeals, const SEXP & aParams ) {
    
    const TBarSeries lBars = XtsToBarSeries( aBars );
    
    const Rcpp::List lParam( aParams );
    const double lMinDealsPerDay = Rcpp::as<double>( lParam[ "MinDeals" ] );
    double lBeginLabel = ( Rcpp::as<Rcpp::Datetime>( lParam["BeginLabel"] ) ).getFractionalTimestamp() * gOneDay;
    double lEndLabel = ( Rcpp::as<Rcpp::Datetime>( lParam["EndLabel"] ) ).getFractionalTimestamp() * gOneDay;
    
    const size_t lMinDeals = std::max( gMinDealsForAllTime, RoundToSize_t( lMinDealsPerDay * ( lEndLabel / gOneDay - lBeginLabel / gOneDay ) ) );

    TPrice lMaxDD = 0.0;
    TPrice lPnl = 0.0;
    size_t lMaxPos = 0;
    size_t lMeadPos = 0;
    const TDeals lDeals( DataFrameToDeals( aDeals ) );
    const TPrice lCoeff = DealsToCoeff( lBars, lDeals, lMinDeals, lPnl, lMaxDD, lMaxPos, lMeadPos );
        
    return Rcpp::List::create(
        Rcpp::Named( "Attraction" ) = lCoeff,
        Rcpp::Named( "MaxDD" ) = lMaxDD,
        Rcpp::Named( "Pnl" ) = lPnl,
        Rcpp::Named( "MaxPos" ) = lMaxPos,
        Rcpp::Named( "MeadPos" ) = lMeadPos
    );
}

//------------------------------------------------------------------------------------------
Rcpp::List DI( const Rcpp::NumericMatrix &aXts, const int aPeriod ) {

    const TBarSeries lBars( XtsToBarSeries( aXts ) );

    TPriceSeries lDMIp;
    TPriceSeries lDMIn;

    if( _DI( lBars, aPeriod, lDMIp, lDMIn ) ) {

        const std::string lTZone( Rcpp::as< std::string >( aXts.attr("tzone") ) );
        ///\todo заменить плохие значения на NA
        return Rcpp::List::create(
            Rcpp::Named("DMp") = PriceSeriesToXts( lDMIp, lTZone ),
            Rcpp::Named("DMn") = PriceSeriesToXts( lDMIn, lTZone )
        );

    } else {
        return R_NilValue;
    }
}

//------------------------------------------------------------------------------------------
Rcpp::List RollMinMax( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const bool aTouch ) {
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );

    TPriceSeries lMin;
    TPriceSeries lMax;

    if( _RollMinMax( lBars, aPeriod, lMin, lMax, aTouch ) ) {
        const std::string lTZone( getTimeZone( aOHLCV ) );
        return Rcpp::List::create(
            Rcpp::Named("Min") = PriceSeriesToXts( lMin, lTZone ),
            Rcpp::Named("Max") = PriceSeriesToXts( lMax, lTZone ),
            Rcpp::Named("MinVolume") = PriceSeriesVolumeToXts( lMin, lTZone ),
            Rcpp::Named("MaxVolume") = PriceSeriesVolumeToXts( lMax, lTZone )
        );

    } else {
        return R_NilValue;
    }
}

//------------------------------------------------------------------------------------------
Rcpp::List RollMinMax_old( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod, const bool aTouch ) {
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );

    TPriceSeries lMin;
    TPriceSeries lMax;

    if( _RollMinMax_old( lBars, aPeriod, lMin, lMax, aTouch ) ) {
        const std::string lTZone( getTimeZone( aOHLCV ) );
        return Rcpp::List::create(
            Rcpp::Named("Min") = PriceSeriesToXts( lMin, lTZone ),
            Rcpp::Named("Max") = PriceSeriesToXts( lMax, lTZone ),
            Rcpp::Named("MinVolume") = PriceSeriesVolumeToXts( lMin, lTZone ),
            Rcpp::Named("MaxVolume") = PriceSeriesVolumeToXts( lMax, lTZone )
        );

    } else {
        return R_NilValue;
    }
}

//------------------------------------------------------------------------------------------
Rcpp::List SupportRessistance( 
    const Rcpp::NumericMatrix & aOHLCV, 
    const int aPeriod, 
    const size_t aMinTouch, 
    const double aTollerance ) {
    
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );

    TPriceSeries lMin;
    TPriceSeries lMax;

    if( _SupportRessistance( lBars, aPeriod, aMinTouch, aTollerance, lMin, lMax ) ) {
        const std::string lTZone( getTimeZone(aOHLCV) );
        return Rcpp::List::create(
            Rcpp::Named("Min") = PriceSeriesToXts( lMin, lTZone ),
            Rcpp::Named("Max") = PriceSeriesToXts( lMax, lTZone ),
            Rcpp::Named("MinVolume") = PriceSeriesVolumeToXts( lMin, lTZone ),
            Rcpp::Named("MaxVolume") = PriceSeriesVolumeToXts( lMax, lTZone )
        );

    } else {
        return R_NilValue;
    }
}

//------------------------------------------------------------------------------------------
Rcpp::List ForwardMinMax( const Rcpp::NumericMatrix & aOHLCV, const int aTimeDelta ) {

    if( aTimeDelta < 1 ) {
        return R_NilValue;
    }
    
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );

    TPriceSeries lMin;
    TPriceSeries lMax;

    if( _ForwardMinMax( lBars, static_cast<size_t>(aTimeDelta), lMin, lMax ) ) {

        const std::string lTZone(getTimeZone(aOHLCV));
        return Rcpp::List::create(
            Rcpp::Named("Min") = PriceSeriesToXts( lMin, lTZone ),
            Rcpp::Named("Max") = PriceSeriesToXts( lMax, lTZone )
        );

    } else {
        return R_NilValue;
    }
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector ChannelSize( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod ) {
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );
    const TPriceSeries lResult( _ChannelSize( lBars, aPeriod ) );
    
    const std::string lTZone(getTimeZone(aOHLCV));
    return PriceSeriesToXts( lResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector SAR( const Rcpp::NumericMatrix & aOHLCV, const double aAccFactor, const double aMaxAccFactor, const bool aOvernight ) {
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );
    const TPriceSeries lResult(
        aOvernight 
        ? 
        _ParabolicSar( lBars, aAccFactor, aMaxAccFactor )
        :
        _IntradayParabolicSar( lBars, aAccFactor, aMaxAccFactor ) 
    );

    const std::string lTZone(getTimeZone(aOHLCV));
    return PriceSeriesToXts( lResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector ZigZag( const Rcpp::NumericMatrix & aOHLCV, const double aGap ) {
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );
    const TPriceSeries lResult( _ZigZag( lBars, aGap ) );
    
    const std::string lTZone(getTimeZone(aOHLCV));
    return PriceSeriesToXts( lResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector AbsoluteZigZag( const Rcpp::NumericMatrix & aOHLCV, const double aGap ) {
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );
    const TPriceSeries lResult( _AbsoluteZigZag( lBars, aGap ) );
    
    const std::string lTZone(getTimeZone(aOHLCV));
    return PriceSeriesToXts( lResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector Stochastic( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod ) {
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );
    const TPriceSeries lResult( _Stochastic( lBars, aPeriod ) );
    
    const std::string lTZone(getTimeZone(aOHLCV));
    return PriceSeriesToXts( lResult, lTZone );
}

//------------------------------------------------------------------------------------------
Rcpp::NumericVector ADX( const Rcpp::NumericMatrix & aOHLCV, const int aPeriod ){
    const TBarSeries lBars( XtsToBarSeries( aOHLCV ) );
    const TPriceSeries lResult( _ADX( lBars, aPeriod ) );
    
    const std::string lTZone(getTimeZ