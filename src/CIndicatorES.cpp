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
