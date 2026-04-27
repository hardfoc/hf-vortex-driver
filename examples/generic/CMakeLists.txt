cmake_minimum_required(VERSION 3.16)
project(hf_vortex_driver_generic LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(vortex_driver_generic_check
  main.cpp
  ../../src/VortexHostSession.cpp
)
target_include_directories(vortex_driver_generic_check PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
)
