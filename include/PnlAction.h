
/**
 * \file PnlAction.h
 * \brief Модуль хранящий в себе типы периодов для баров и основные методы работы с ними
 * \author kan <kansoftware.ru>
 * \since 2015-11-11
 * \date 2021-11-08
 * Модуль используется неизменно в проектах TradeBot и [R] BackTester
 */

#ifndef BACKTESTER_PNLACTION_H
#define BACKTESTER_PNLACTION_H

#include <list>
#include <vector>
#include <map>
#include <string>

#include "BasisOfStrategy.h"

const double gMaxBedAttraction = -99.9;
const double gFiltredByCount = -33.3;
const double gFiltredByCountPnl = -66.6;
const double gFiltredByPnl = 0.0;

const size_t gMinDealsForAllTime = 6UL;
const size_t gMinDealsStatictics = 30UL;
const size_t gWellDealsStatictics = 1000UL;

typedef std::map<std::string, double> TValues;

struct TDeal {
    TInnerDate OpenTime = 0.0;
    TPrice OpenPrice = GetBadPrice();
    TPrice StopLoss = GetBadPrice();
    TPrice TakeProfit = GetBadPrice();
    TPrice ClosePrice = GetBadPrice();
    TInnerDate CloseTime = 0.0;
    TDealSide DealSide = TDealSide::None;
    size_t Volume = 0;
    TValues Additive;

    TDeal(){;}

    TDeal(
        const TInnerDate aOpenTime,
        const TPrice aOpenPrice,
        const TPrice aStopLoss,
        const TPrice aTakeProfit,
        const TPrice aClosePrice,
        const TInnerDate aCloseTime,
        const TDealSide aDealSide,
        const size_t aVolume = 1,
        const TValues aAdditive={} ) :
        OpenTime( aOpenTime ),
        OpenPrice( aOpenPrice ),
        StopLoss( aStopLoss ),
        TakeProfit( aTakeProfit ),
        ClosePrice( aClosePrice ),
        CloseTime( aCloseTime ),
        DealSide( aDealSide ),
        Volume( aVolume ),
        Additive( aAdditive ){;}

    void Reset() {
        OpenTime = 0.0;
        ResetPrice( OpenPrice );
        ResetPrice( ClosePrice );
        CloseTime = 0.0;
        DealSide = TDealSide::None;