/**
 * \file Indicators.h
 * \brief Модуль реализующий индикаторы для одноногих стратегий
 * \author kan <kansoftware.ru>
 * \since 2015-11-10
 * \date 2019-07-31
 * Модуль используется неизменно в проектах TradeBot и [R] BackTester
 */

#ifndef BACKTESTER_INDICATORS_H
#define BACKTESTER_INDICATORS_H

#include "BasisOfStrategy.h"
#include "BarPeriod.h"

const size_t gUsingCPU = 4;

TPriceSeries _TrueRange( const TBarSeries & aBars );
TPriceSeries _AverageTrueRange( const TBarSeries & aBars, const int aPeriod );

bool _DM( const TBarSeries & aBars, 
            TPriceSeries & aoDMp, 
            TPriceSeries & aoDMn );
bool _DI( const TBarSeries & aBars, const int aPeriod, 
            TPriceSeries & aoDMIp, 
            TPriceSeries & aoDMIn );

TPriceSeries _ADX( const TBarSeries & aBars, const int aPeriod );

TPriceSeries _SimpleMA( const TPriceSeries & aPrices, const int aPeriod, const size_t aLag = 0 );
TPriceSeries _ExponentMA( const TPriceSeries & aPrices, const int aPeriod, const size_t aLag = 0 );
TPriceSeries _SmoothedMA( const TPriceSeries & aPrices, const int aPeriod, const size_t aLag = 0 );
TPriceSeries _KAMA( co