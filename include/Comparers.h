/**
 * \file Comparers.h
 * \brief Модуль содержащий методы для сравнения чисел с заданной точностью
 * \author kan <kansoftware.ru>
 * \since 2015-08-25
 * \date 2020-02-28
 */

#ifndef COMPARERS_H
#define	COMPARERS_H

constexpr double gAbsoluteZero = 0.000000001;/*!< 0.000001 Числовное представление "маленького" значения */

/**
 *  \brief Проверяемое число, чуток, но больше чем 0
 */
template< typename TData > constexpr bool isPositiveValue( const TData A, const double aAbsoluteZero = gAbsoluteZero ) {
  