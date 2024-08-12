cmake_minimum_required(VERSION 3.22)
project(siyi-sdk_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib)
add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_subdirectory(include/crc)

add_library(
    siyi-sdk
    src/message.cpp
    src/sdk.cpp
    src/gimbal.cpp
    src/ai.cpp
)
target_link_libraries(siyi-sdk PUBLIC gencrc)
target_include_directories(siyi-sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    include(CTest)
    add_subdirectory(src/tests)
endif()
