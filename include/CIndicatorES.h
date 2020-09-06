
/* 
 * File:   CIndicatorES.h
 * Author: https://www.mql5.com/ru/articles/318
 *
 * Created on January 20, 2017, 4:14 PM
 */

#ifndef CINDICATORES_H
#define CINDICATORES_H

#include "PowellsMethod.h"

struct TForecastModelParams{
    double S;
    double t;
    double Alpha;
    double Gamma;
    double Phi;
    
    void SetDefault( const double aS ) {
        S = aS;
        t = 0.0;
        Alpha = 0.5;