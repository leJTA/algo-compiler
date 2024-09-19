cmake_minimum_required(VERSION 3.30)

project(algo_compiler)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE})

add_definitions(-DBOOST_PHOENIX_NO_VARIADIC_EXPRESSION)

file(GLOB SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HDRS ${CMAKE_SOURCE_DIR}/include/*.hpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(algo_compiler 
   ${SRCS} 
   ${HDRS}
)

target_link_libraries(${PROJECT_NAME} 
   PUBLIC Boost::boost
)
