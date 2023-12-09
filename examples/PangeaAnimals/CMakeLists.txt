PROJECT(PangeaAnimals)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/examples/Zoo")

add_library(PangeaAnimals SHARED plugin.cpp Cat.h Dog.h)