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
#define SIGN(a,b)