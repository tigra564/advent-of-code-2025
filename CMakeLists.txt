cmake_minimum_required(VERSION 3.10)
project(AoC2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

set(CMAKE_DEPENDS_USE_COMPILER ON)

add_executable(01-1 01-1.cpp common.cpp)
add_executable(01-2 01-2.cpp common.cpp)

add_executable(02-1 02-1.cpp common.cpp)
add_executable(02-2 02-2.cpp common.cpp)

add_executable(03-1 03-1.cpp common.cpp)
add_executable(03-2 03-2.cpp common.cpp)

add_executable(04-1 04-1.cpp common.cpp)
add_executable(04-2 04-2.cpp common.cpp)

add_executable(05-1 05-1.cpp common.cpp)
add_executable(05-2 05-2.cpp common.cpp)
add_executable(05-2-boost 05-2-boost.cpp common.cpp)

add_executable(06-1 06-1.cpp common.cpp)
add_executable(06-2 06-2.cpp common.cpp)

add_executable(07-1 07-1.cpp common.cpp)
add_executable(07-2 07-2.cpp common.cpp)

add_executable(08-1 08-1.cpp common.cpp 08-common.cpp)
add_executable(08-2 08-2.cpp common.cpp 08-common.cpp)

add_executable(09-1 09-1.cpp common.cpp)
add_executable(09-2 09-2.cpp common.cpp)
add_executable(09-2-boost 09-2-boost.cpp)

add_executable(10-1 10-1.cpp common.cpp 10-common.cpp)
add_executable(10-2 10-2.cpp common.cpp 10-common.cpp ilp-solver.cpp)

add_executable(11-1 11-1.cpp common.cpp 11-common.cpp)
add_executable(11-2 11-2.cpp common.cpp 11-common.cpp)

add_executable(12-1 12-1.cpp common.cpp)
