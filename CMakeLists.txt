cmake_minimum_required(VERSION 3.20)
project(posey-pc VERSION 0.1.0)

# Do not allow in-source builds:
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)

file(GLOB_RECURSE POSEY_LIB_SRCS
    ${CMAKE_SOURCE_DIR}/posey-lib/*.cpp)
add_library(PoseyLib ${POSEY_LIB_SRCS})
target_link_libraries(PoseyLib fmt::fmt)
target_include_directories(PoseyLib PUBLIC
    ${CMAKE_SOURCE_DIR}/posey-lib)
target_include_directories(PoseyLib PRIVATE
    ${CMAKE_SOURCE_DIR}/sim/src)

file(GLOB_RECURSE POSEY_SIM_SRCS
    ${CMAKE_SOURCE_DIR}/sim/src/*.cpp)
add_executable(PoseySim ${POSEY_SIM_SRCS})
target_include_directories(PoseySim PUBLIC
    ${CMAKE_SOURCE_DIR}/posey-lib
    ${CMAKE_SOURCE_DIR}/sim/src)
target_link_libraries(PoseySim PoseyLib fmt::fmt)

file(GLOB_RECURSE POSEY_TEST_SRCS
    ${CMAKE_SOURCE_DIR}/test/*.cpp)
add_executable(PoseyTest ${POSEY_TEST_SRCS})
target_link_libraries(PoseyTest PoseyLib Catch2::Catch2 fmt::fmt)
target_include_directories(PoseyTest PUBLIC
    ${CMAKE_SOURCE_DIR}/posey-lib
    ${CMAKE_SOURCE_DIR}/sim/src
    ${CMAKE_SOURCE_DIR}/test)

add_subdirectory(python)

include(CTest)
include(Catch)
catch_discover_tests(PoseyTest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

