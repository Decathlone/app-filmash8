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
TPriceSeries _AverageTrueRange( const TBarSeries &