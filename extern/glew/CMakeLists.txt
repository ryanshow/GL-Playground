cmake_minimum_required (VERSION 2.6)

project (glew)

add_definitions (-DGLEW_STATIC)

include_directories(include)

add_library (glew STATIC glew.c)
