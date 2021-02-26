// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* 
 * File:   CIndicatorES.cpp
 * Author: victorg(mq5), kan(c++)
 * 
 * Created on January 20, 2017, 4:14 PM
 */
#include <cmath>
#include "DelphisRound.h"

#include "CIndicatorES.h"

//-----------------------------------------------------------------------------------
// CalcPar
//-----------------------------------------------------------------------------------
double CIndicatorES::GetPar(const int n) {
    if (n >= 0 and n < 5) {
        return ( Par[ ToSize_t(n) ]);
    } else {
        return ( 0.0);
    }
}

//-----------------------------------------------------------------------------------
bool CIndicatorES::CalcPar(const TDoubles &dat, const TForecastModelParams &aParams) {
    Dlen = dat.size();
    Dat = dat;

    Par[0] = aParams.S; // Dat[0]; // initial S
    Par[1] = aParams.t; // 0; // initial T
    Par[2] = aParams.Alpha; // 0.5; // initial Alpha
    Par[3] = aParams.Gamma; // 0.5; // initial Gamma
    Par[4] = aParams.Phi; // 0.5; // initial Phi

    return Optimize(Par) > 0.0; // Powell's optimization
}

//------------------------------------------------------------------------------------
// func
//------------------------------------------------------------------------------------
double CIndicatorES::func(const TDoubles &p) {
    double s, t, alp, gam, phi, k1, k2, k3, e, sse, ae, pt;

 