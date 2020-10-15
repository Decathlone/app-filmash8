/* 
 * File:   Forecasting.h
 * Author: kan <kansoftware.ru>
 *
 * Created on January 23, 2017, 1:58 PM
 */

#ifndef FORECASTING_H
#define FORECASTING_H

#include "CIndicatorES.h"

struct TForecastResult{
    double lower;
    double forecast;
    double upper;
} ;

// To calc confidence intervals 