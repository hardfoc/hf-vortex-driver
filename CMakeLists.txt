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

# The documentation code (tests/docs), compiled and checked on the host.
option(HF_VORTEX_BUILD_DOC_SNIPPETS "Build and test the code shown on the documentation site" OFF)
if(HF_VORTEX_BUILD_DOC_SNIPPETS)
  enable_testing()
  add_subdirectory(tests/docs)
endif()

# Host unit tests (tests/protocol): the protocol constants against CiA 301 / CiA 402.
option(HF_VORTEX_BUILD_TESTS "Build and run the host unit tests" OFF)
if(HF_VORTEX_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests/protocol)
endif()
