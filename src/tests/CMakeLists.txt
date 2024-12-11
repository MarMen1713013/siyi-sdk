cmake_minimum_required(VERSION 3.22)
project(siyi-sdk_tests)

add_executable(crc_test crc_test.cpp)
target_link_libraries(crc_test PUBLIC siyi-sdk)
add_test(NAME crc_test COMMAND crc_test)

add_executable(four_point_test four_point_test.cpp)
target_link_libraries(four_point_test PUBLIC siyi-sdk)

add_executable(tracker_test tracker_test.cpp)
target_link_libraries(tracker_test PUBLIC siyi-sdk)

add_executable(gimbal_control gimbal_control.cpp)
target_link_libraries(gimbal_control PUBLIC siyi-sdk)

add_executable(remote_joy remote_joy.cpp)
target_link_libraries(remote_joy PUBLIC siyi-sdk)
