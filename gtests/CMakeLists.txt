
cmake_minimum_required( VERSION 2.8 )
project( TradingBasics_tests )

find_package( Threads REQUIRED )

set( GOOGLE_SOURCE_FILES "$ENV{HOME}/cpp/googletest" )
include_directories( "${GOOGLE_SOURCE_FILES}/googletest/include" )
link_directories( "${GOOGLE_SOURCE_FILES}/build/lib" )

set( TRADING_BASICS_PATH "$ENV{HOME}/cpp/companents/TradingBasics/" )