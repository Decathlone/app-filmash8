# Trading basics library

## About

This is classes, types and functions for creating trading strategies and bots with C++ compilers.

## Depends

git cmake gcc g++ libboost-math-dev

## Installing

```bash
git clone https://github.com/kansoftware/TradingBasics.git
cd TradingBasics
mkdir build; cd build
cmake -DCMAKE_C_FLAGS="-O3 -march=native -DNDEBUG" -DCMAKE_CXX_FLAGS="-O3 -march=native -DNDEBUG" ..
cmake --build . -- -j 4

```

For correct work need 