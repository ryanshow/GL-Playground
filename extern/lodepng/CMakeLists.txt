cmake_minimum_required (VERSION 2.6)

project (lodepng)

include_directories(include)

add_library (lodepng STATIC lodepng.cpp)
