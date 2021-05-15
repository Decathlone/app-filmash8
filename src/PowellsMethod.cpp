// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/** 
 * File:   PowellsMethod.cpp
 * Author: victorg(mq5), kan(c++)
 * 
 * Created on January 20, 2017, 3:58 PM
 */

#include <stdio.h> 
#include <math.h>

#include "PowellsMethod.h"

#define GOLD   1.618034
#define CGOLD  0.3819660
#define GLIMIT 100.0
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define SIGN(a,b) (((b) >= 0.0) ? fabs(a) : -fabs(a))
#define FMAX(a,b) ((a>b)?a:b)

//-----------------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------------
PowellsMethod::PowellsMethod() {
    ItMaxPowell = 200;
    FtolPowell = 1e-6;
    ItMaxBrent = 200;
    FtolBrent = 1e-4;
}

//-----------------------------------------------------------------------------------
void PowellsMethod::powell() {
    size_t m, n, ibig;
    double del, fp, fptt, t;

    n = N;
    Fret = func(P);
    for (size_t j = 0; j < n; j++)Pt[j] = P[j];
    for (Iter = 1;; Iter++) {
        fp = Fret;
        ibig = 0;
        del = 0.0;
        for