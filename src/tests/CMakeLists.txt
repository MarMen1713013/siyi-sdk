cmake_minimum_required(VERSION 3.22)
project(siyi-sdk_tests)

add_executable(crc_test crc_test.cpp)
target_link_libraries(crc_test PUBLIC siyi-sdk)
add_test(NAME crc_test COMMAND crc_test)

add_executable(four_point_test four_point_test.cpp)
target_link_libraries(four_point_test PUBLIC siyi-sdk)
