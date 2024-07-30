cmake_minimum_required(VERSION 3.22)
project(siyi-sdk_tests)

add_executable(crc_test crc_test.cpp)
target_link_libraries(crc_test PUBLIC siyi-sdk gencrc)
add_test(NAME crc_test COMMAND crc_test)
