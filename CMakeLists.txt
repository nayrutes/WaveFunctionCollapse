cmake_minimum_required(VERSION 3.27)
project(WaveCollapseFunction)

set(CMAKE_CXX_STANDARD 20)

add_executable(WaveCollapseFunction main.cpp
        Image.h
        Image.cpp
        WfcCore.cpp
        WfcCore.h
        Rules.cpp
        Rules.h)
