cmake_minimum_required(VERSION 3.6)

project(io_uring-reactor)

add_subdirectory(lib)

add_executable(echo echo.cpp)

target_include_directories(echo PUBLIC lib)
target_link_libraries(echo PUBLIC reactor)

add_executable(timer timer.cpp)

target_include_directories(timer PUBLIC lib)
target_link_libraries(timer PUBLIC reactor)