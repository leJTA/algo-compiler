cmake_minimum_required(VERSION 3.30)

project(algo-compiler)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost)

if (NOT CMAKE_BUILD_TYPE)
   message("CMAKE_BUILD_TYPE not specified. Defaulting to 'Release'.")
   set(CMAKE_BUILD_TYPE Release)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
add_compile_definitions(-DBOOST_PHOENIX_NO_VARIADIC_EXPRESSION)
add_compile_options(-O3)

set(SRCS
   ${CMAKE_SOURCE_DIR}/src/compiler.cpp
   ${CMAKE_SOURCE_DIR}/src/constant_declaration.cpp
   ${CMAKE_SOURCE_DIR}/src/expression.cpp
   ${CMAKE_SOURCE_DIR}/src/function_definition.cpp
   ${CMAKE_SOURCE_DIR}/src/main.cpp
   ${CMAKE_SOURCE_DIR}/src/program.cpp
   ${CMAKE_SOURCE_DIR}/src/statement.cpp
   ${CMAKE_SOURCE_DIR}/src/variable_declaration.cpp
   ${CMAKE_SOURCE_DIR}/src/vm.cpp
)

set(HDRS 
   ${CMAKE_SOURCE_DIR}/src/annotation.hpp
   ${CMAKE_SOURCE_DIR}/src/ast.hpp
   ${CMAKE_SOURCE_DIR}/src/compiler.hpp
   ${CMAKE_SOURCE_DIR}/src/constant_declaration.hpp
   ${CMAKE_SOURCE_DIR}/src/error_handler.hpp
   ${CMAKE_SOURCE_DIR}/src/expression.hpp
   ${CMAKE_SOURCE_DIR}/src/expression_def.hpp
   ${CMAKE_SOURCE_DIR}/src/function_definition.hpp
   ${CMAKE_SOURCE_DIR}/src/function_definition_def.hpp
   ${CMAKE_SOURCE_DIR}/src/program.hpp
   ${CMAKE_SOURCE_DIR}/src/program_def.hpp
   ${CMAKE_SOURCE_DIR}/src/skipper.hpp
   ${CMAKE_SOURCE_DIR}/src/statement.hpp
   ${CMAKE_SOURCE_DIR}/src/statement_def.hpp
   ${CMAKE_SOURCE_DIR}/src/type_setting.hpp
   ${CMAKE_SOURCE_DIR}/src/variable_declaration.hpp
   ${CMAKE_SOURCE_DIR}/src/vm.hpp
)

add_executable(compiler
   ${SRCS} 
   ${HDRS}
)

target_link_libraries(compiler 
   PUBLIC Boost::boost
)