
/**
 * \file DelphisRound.h
 * \brief Модуль содержащий методы для округления с приведением типа, как в Дельфи
 * \author kan <kansoftware.ru>
 * \since 2015-08-25
 * \date 2019-09-23
 */

#ifndef DELPHISROUND_H
#define	DELPHISROUND_H

#include <cmath>
#include <limits>
#include <stdexcept>
#include <cassert>

#include "Comparers.h"


const double gMaxInteger = static_cast< double >( std::numeric_limits<int>::max() );
const double gMinInteger = static_cast< double >( std::numeric_limits<int>::min() );
const double gMaxSizet = static_cast< double >( std::numeric_limits<size_t>::max() );

/**
 *  \fn ToDouble
 *  \brief приведение к double
 */
#ifdef NDEBUG
    #define ToDouble( aValue ) static_cast< double >( (aValue) )
#else
    template< typename T > double ToDouble( const T aValue ) {
        return static_cast< double >( aValue );
    }
#endif

/**
 *  \fn ToSize_t
 *  \brief приведение size_t , отрицательные значения заменяются на 0
 */
template< typename T > constexpr size_t ToSize_t( const T aValue ) {
    assert( ToDouble( aValue ) < gMaxSizet  );
    
    return isPositiveValue( ToDouble(aValue) ) ? static_cast< size_t >( aValue ) : 0UL ;
}

/**
 *  \fn ToInt
 *  \brief приведение к Int, если значение вне диапазона Int будет except
 */
#ifdef NDEBUG
    #define ToInt( aValue ) static_cast< int >( (aValue) )
#else
    template< typename T > int ToInt( const T aValue ) {
        assert( gMinInteger <= ToDouble( aValue ) );
        assert( ToDouble( aValue ) <= gMaxInteger );

        return static_cast< int >( aValue );
    }
#endif

/**
 *  \fn Round
 *  \brief Округление как в Delphi из double в int
 *  \todo всё с округлением вынести в отдельный модуль
 */
#ifdef NDEBUG
    #define Round( aValue ) static_cast< int >( std::round( aValue ) )
#else    
    template< typename T > int Round( const T aValue ) {
        assert( gMinInteger <= ToDouble( aValue ) );
        assert( ToDouble( aValue ) <= gMaxInteger );

        return static_cast< int >( std::round( aValue ) );
    }
#endif

/**
 *  \fn RoundToSize_t
 *  \brief Округление double в size_t , отрицательные значения заменяются на 0
 */