# Optional standalone static library (host or embedded consumers)
cmake_minimum_required(VERSION 3.16)
project(hf_vortex_driver LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(hf_vortex_driver STATIC
  src/VortexHostSession.cpp
)
target_include_directories(hf_vortex_driver PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Generic host test: add_subdirectory(examples/generic) from parent if desired
