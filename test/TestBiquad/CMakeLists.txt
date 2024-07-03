cmake_minimum_required(VERSION 3.16)
project("TestBiquad")

set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ version selection")  # or 11, 14, 17, 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # optional, ensure standard is supported

set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/source")
set(EXTERNALS "${PROJECT_SOURCE_DIR}/externals")

# find_package(Python COMPONENTS Interpreter Development)
set(PYTHON_EXECUTABLE "C:/Program Files/Python312/python.exe")

# PyBind11
add_subdirectory(${EXTERNALS}/pybind11)

pybind11_add_module(biquadWrapper ${MODULE_SOURCE}/biquadWrapper.cpp)

target_include_directories(
    biquadWrapper
    PRIVATE ${MODULE_SOURCE}
)

target_link_libraries(biquadWrapper PRIVATE pybind11::embed)