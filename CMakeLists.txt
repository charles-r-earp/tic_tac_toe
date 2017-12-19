cmake_minimum_required (VERSION 3.9)
project(tic-tac-toe)

if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 17)
endif ()

add_executable(tic_tac_toe tic_tac_toe.cpp)
target_include_directories(tic_tac_toe PRIVATE include)