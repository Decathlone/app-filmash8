
/*
* \file FileManipulator.h
* \brief Модуль шаблон для работы с типизированными файлами
* \author https://habrahabr.ru/post/134788/
* \since 2016-09-29
* \date 2016-11-03
*/

#ifndef KELTHNERCUDABACKTESTER_FILEMANIPULATOR_H
#define KELTHNERCUDABACKTESTER_FILEMANIPULATOR_H
#include <iostream>

using std::istream;
using std::ostream;

template<typename T>
class TWrapFileData {
    T t;