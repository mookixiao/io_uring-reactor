cmake_minimum_required(VERSION 3.6)

project(io_uring-reactor)

add_subdirectory(lib)

add_executable(echo echo.cpp)

target_include_directories(echo PRIVATE lib)
target_link_libraries(echo PRIVATE reactor)