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
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++)Xit[j] = Xi[j + n * i];
            fptt = Fret;
            linmin();
            if (fabs(fptt - Fret) > del) {
                del = fabs(fptt - Fret);
                ibig = i;
            }
        }

        if (2.0 * fabs(fp - Fret) <= FtolPowell * (fabs(fp) + fabs(Fret) + 1e-25))return;

        if (Iter >= ItMaxPowell) {
            printf("powell exceeding maximum iterations!");
            MaxIterFlag = 1;
            return;
        }

        for (size_t j = 0; j < n; j++) {
            Ptt[j] = 2.0 * P[j] - Pt[j];
            Xit[j] = P[j] - Pt[j];
            Pt[j] = P[j];
        }

        fptt = func(Ptt);
        if (fptt < fp) {
            t = 2.0 * (fp - 2.0 * (Fret) + fptt)*(fp - Fret - del)*(fp - Fret - del) - del * (fp - fptt)*(fp - fptt);
            if (t < 0.0) {
                linmin();
                for (size_t j = 0; j < n; j++) {
                    m = j + n * (n - 1);
                    Xi[j + n * ibig] = Xi[m];
                    Xi[m] = Xit[j];
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------
void PowellsMethod::linmin() {
    double xx, xmin, fx, fb, fa, bx, ax;

    size_t n = N;
    for (size_t j = 0; j < n; j++) {
        Pcom[j] = P[j];
        Xicom[j] = Xit[j];
    }
    ax = 0.0;
    xx = 1.0;
    mnbrak(ax, xx, bx, fa, fx, fb);
    Fret = brent(ax, xx, bx, xmin);
    for (size_t j = 0; j < n; j++) {
        Xit[j] *= xmin;
        P[j] += Xit[j];
    }
}

//-----------------------------------------------------------------------------------
void PowellsMethod::mnbrak(
    double &ax,
    double &bx,
    double &cx,
    double &fa,
    double &fb,
    double &fc) {
    
    double ulim, u, r, q, fu, dum;

    fa = f1dim(ax);
    fb = f1dim(bx);
    if (fb > fa) {
        SHFT(dum, ax, bx, dum)
        SHFT(dum, fb, fa, dum)
    }
    cx = bx + GOLD * (bx - ax);
    fc = f1dim(cx);
    while (fb > fc) {
        r = (bx - ax)*(fb - fc);
        q = (bx - cx)*(fb - fa);
        u = bx - ((bx - cx) * q - (bx - ax) * r) / (2.0 * SIGN(FMAX(fabs(q - r), 1e-20), q - r));
        ulim = bx + GLIMIT * (cx - bx);
        if ((bx - u)*(u - cx) > 0.0) {
            fu = f1dim(u);
            if (fu < fc) {
                ax = bx;
                bx = u;
                fa = fb;
                fb = fu;
                return;
            } else if (fu > fb) {
                cx = u;
                fc = fu;
                return;
            }
            u = cx + GOLD * (cx - bx);
            fu = f1dim(u);
        } else if ((cx - u)*(u - ulim) > 0.0) {
            fu = f1dim(u);
            if (fu < fc) {
                SHFT(bx, cx, u, cx + GOLD * (cx - bx))
                SHFT(fb, fc, fu, f1dim(u))
            }
        } else if ((u - ulim)*(ulim - cx) >= 0.0) {
            u = ulim;
            fu = f1dim(u);
        } else {
            u = cx + GOLD * (cx - bx);
            fu = f1dim(u);
        }
        SHFT(ax, bx, cx, u)
        SHFT(fa, fb, fc, fu)
    }
}

