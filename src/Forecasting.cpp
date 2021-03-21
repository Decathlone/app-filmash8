// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* 
 * File:   Forecasting.cpp
 * Author: victorg(mq5), kan(c++)
 * 
 * Created on January 23, 2017, 1:58 PM
 */

#include <cmath>
#include <cstddef>

#include "Prices.h"
#include "DelphisRound.h"

#include "Forecasting.h"

//-----------------------------------------------------------------------------------
TForecastResult Forecast(
    const TDoubles &aData, 
    TForecastModelParams & aioParams, 
    const double aConfidenceIntervals ) {
    
    const size_t NDat = aData.size();

    CIndicatorES Es;
    if (not Es.CalcPar(aData, aioParams)) {
        return TForecastResult{ GetBa