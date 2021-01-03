
/** 
 * \file VolatilityBarBuffer.h
 * \author kan <kansoftware.ru>
 * \since 2020-06-23
 * \date 2020-06-23
 * \brief По заранее заполненному буферу рассчитывает min/max/ATR/err(TR), полный аналог VolatilityBuffer.h
 */

#ifndef VOLATILITYBARBUFFER_H
#define VOLATILITYBARBUFFER_H

#include <cmath>
#include <vector>
#include <set>
#include <deque>

#include "BasisOfStrategy.h"

class TVolatilityBarBuffer {
    public:
        using TBufferType = std::vector<double>;
        explicit TVolatilityBarBuffer( const size_t aCount ):
            fCount(aCount),fBuffer() {
                assert( fCount > 0 );
                fBuffer.reserve(fCount);
            }

        bool isFill() const {return fBuffer.size() == fCount;}
        void reset(){
            fBuffer.clear();
            fMax = NAN;
            fMin = NAN;
            fMean = NAN;
            fErr = NAN;
        }
        bool add(const TSimpleBar& aValue) {
            if( not isFill() ){
                const double lValue = aValue.High - aValue.Low;
                fBuffer.emplace_back(lValue);
                if(not std::isnan(fMax)){
                    fMax = IsGreat(aValue.High, fMax) ? aValue.High : fMax;
                    fMin = IsLess(aValue.Low, fMin) ? aValue.Low : fMin;
                    fMean += lValue;
                }else{
                    fMax = aValue.High;
                    fMin = aValue.Low;
                    fMean = lValue;
                }

                const bool lisfill = isFill();
                if(lisfill){
                    const double lc = static_cast<double>(fCount);
                    const double lMean = fMean / lc;
                    double lsd = 0.0;
                    for (auto& lv : fBuffer) {
                        lsd += pow( lMean - lv, 2 );
                    }
                    
                    lsd /= (lc-1.5);
                    fErr = sqrt(lsd) / sqrt(lc);
                }

                return lisfill;