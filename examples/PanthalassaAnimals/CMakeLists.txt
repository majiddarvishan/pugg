PROJECT(PanthalassaAnimals)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/examples/Zoo")

add_library(PanthalassaAnimals SHARED plugin.cpp Fish.h Whale.h)