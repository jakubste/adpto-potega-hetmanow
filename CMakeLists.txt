cmake_minimum_required(VERSION 3.5)
project(adpto_potega_hetmanow)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES adpto.cpp)

add_executable(adpto_potega_hetmanow ${SOURCE_FILES})