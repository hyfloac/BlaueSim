cmake_minimum_required(VERSION 3.24)
project(BlaueSim VERSION 0.0.1 LANGUAGES CXX C)

include(CTest)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(BlaueSimViewer)
