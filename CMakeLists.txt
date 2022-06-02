cmake_minimum_required(VERSION 3.22)
project(EpidemicCA)
set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED SDL2)


add_executable(EpidemicCA main.cpp Cell.cpp Cell.h constants.h)

target_link_libraries(EpidemicCA PRIVATE SDL2)