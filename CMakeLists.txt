cmake_minimum_required(VERSION 3.2.1)

project(pugg CXX)

add_library(pugg INTERFACE)

target_include_directories(pugg INTERFACE include/)

include(GNUInstallDirs)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp")

option(BUILD_TESTS    "Build tests"    OFF)
option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
