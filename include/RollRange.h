
/**
 * \file RollRange.h
 * \brief Модуль реализующий перекатывающиеся индикатор Min-Max Range
 * \since 2016-10-10
 * \date 2021-01-20
 * Модуль заимствован с https://bitbucket.org/quanttools/quanttools
 * (+) RollRange_with_tollerance [kan@kansoftware.ru]
 */

#ifndef ROLLRANGE_H
#define ROLLRANGE_H

#include <math.h>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

#include "DelphisRound.h"

struct Range {
    double min;
    double max;
    double quantile;
};

class RollRange {
    private:
        Range range;
        const size_t n;
        const bool backSearch;
        const int pN;
        std::queue< double > window;
        std::multiset< double > windowSorted;

        std::vector< double > minHistory;
        std::vector< double > maxHistory;
        std::vector< double > quantileHistory;
        
        const double fBadValue;
        const size_t fArrayReserve = 1000; 

    public:

        RollRange( const size_t aN, const double aP = 1.0, const double aBadValue = -333333.33 ) :
            n( aN ),
            backSearch(aP >= 0.5),
            pN( (backSearch ? Trunc((aP - 1.0) * ToDouble(n) ) : Trunc(aP * ToDouble(n) ) ) ),
            fBadValue( aBadValue ),