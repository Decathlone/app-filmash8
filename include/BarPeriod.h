/**
 * \file BarPeriod.h
 * \brief Модуль хранящий в себе типы периодов для баров и основные методы работы с ними
 * \author kan <kansoftware.ru>
 * \since 2016-12-19
 * \date 2021-05-31
 * Модуль используется неизменно в проектах TradeBot и [R] BackTester
 */

#ifndef BARPERIOD_H
#define BARPERIOD_H

#include <string>
#include <cassert>

enum class TBarPeriod { 
    minValue = 0,
    Minute = 1,
    Minute5 = 2,
    