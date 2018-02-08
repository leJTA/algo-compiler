cmake_minimum_required(VERSION 2.8)

project(algo_compiler)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DBOOST_PHOENIX_NO_VARIADIC_EXPRESSION)

file(GLOB SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/include/*.hpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SRCS})
