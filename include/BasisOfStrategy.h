
/**
 * \file BasisOfStrategy.h
 * \brief Модуль реализующий объекты для одно-ногих стратегий
 * \author kan <kansoftware.ru>
 * \since 2015-11-11
 * \date 2021-10-29
 * Модуль используется неизменно в проектах TradeBot и [R] BackTester
 */


#ifndef BACKTESTER_BASISOFSTRATEGY_H
#define BACKTESTER_BASISOFSTRATEGY_H

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Prices.h"
#include "BarPeriod.h"

enum class TMAPoint {
    MAP_Open = 0,
    MAP_High = 1,
    MAP_Low = 2,
    MAP_Close = 3,
    MAP_Volume = 4,
    MAP_Mid = 5,
    MAP_Triple = 6
};

enum class TDealSide { None=0, Buy=1, Sell=2, Any };

using TInnerDate = double;
using TInnerTime = double;

const double gOneDay = 86400.0;
const double gOneHour = 3600.0;
const double gStartingTime = 975888000;//GMT: Mon, 04 Dec 2000 00:00:00 GMT

struct TSimpleTick {
    TInnerDate DateTime;
    TPrice Price;
    double Volume;

    TSimpleTick(
        const TInnerDate aDateTime=gStartingTime,
        const TPrice aPrice=0.0,
        const double aVolume=0.0 ):
        DateTime(aDateTime),
        Price(aPrice),
        Volume(aVolume){;}
};

using TPriceSeries = std::vector< TSimpleTick >;
