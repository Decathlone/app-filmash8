
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