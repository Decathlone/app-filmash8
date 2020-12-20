
/**
 * \file RollRangeBar.h
 * \brief Модуль реализующий перекатывающиеся индикатор Min-Max Range для баров
 * \author kan <kansoftware.ru>
 * \since 2020-10-21
 * \date 2020-12-08
 * \note Non thread-safe
 */

#ifndef ROLLRANGEBAR_H
#define ROLLRANGEBAR_H

#include <math.h>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>

#include "DelphisRound.h"
#include "BasisOfStrategy.h"

class TRollRangeBar {
    public:
        using TRange = std::pair<double, double>;//min max

        TRollRangeBar( const size_t aN, const double aBadValue = -333333.33 ) :
            n( aN ),
            fBadValue( std::make_pair(aBadValue,aBadValue) ),
            range( fBadValue),
            fArrayReserve( std::max(aN, 1000UL) ) {
            if (n < 1) throw std::invalid_argument("n must be greater than 0");

            History.reserve( fArrayReserve );
        }

        void Add( const TSimpleBar& value ) {
            window.push( std::make_pair(value.Low, value.High) );
            windowSortedMin.insert(value.Low);
            windowSortedMax.insert(value.High);

            if( window.size() > n ){
                windowSortedMin.erase( windowSortedMin.find(window.front().first) );
                windowSortedMax.erase( windowSortedMax.find(window.front().second) );

                window.pop();
            }
            
            range = std::make_pair( *windowSortedMin.begin(), *windowSortedMax.rbegin() );

            if( IsFormed() ){
                History.push_back( range );
            } else {
                History.push_back( fBadValue );
            }

            fCached_m = false;
            fCached_e = false;
        }

        bool IsFormed() const {
            return window.size() == n;
        }

        TRange GetValue() const {
            return range;
        }

        std::vector< TRange > GetHistory() const {
            return History;
        }

        void Reset() {
            window={};
            windowSortedMax.clear();
            windowSortedMin.clear();

            History.clear();
            History.reserve( fArrayReserve );

            fCached_m = false;
            fCached_e = false;
        }

        double getMean() const {
            if( not IsFormed() ){
                return NAN;
            }
            
            if(fCached_m){
                return fMean;
            }
