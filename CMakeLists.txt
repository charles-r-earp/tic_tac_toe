cmake_minimum_required (VERSION 3.5)
project(tic-tac-toe)

if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_FLAGS -pthread)

add_executable(tic_tac_toe tic_tac_toe.cpp)
target_include_directories(tic_tac_toe PRIVATE include)
