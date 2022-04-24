project(pyposey)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

file(GLOB_RECURSE SRCS src/*.cpp)
pybind11_add_module(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} PUBLIC PoseyLib)
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    ${CMAKE_SOURCE_DIR}/posey-lib)

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO="1.0")
