PROJECT(zoo)

include_directories( "${CMAKE_SOURCE_DIR}/include")

add_executable(zoo main.cpp Animal.h)

target_link_libraries(zoo
    ${CMAKE_DL_LIBS}
)

