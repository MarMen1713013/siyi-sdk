#include "gimbal.hpp"
#include <iostream>
#include <fcntl.h>
#include <cstring>

namespace SIYI {
    SIYIUnix_Gimbal::SIYIUnix_Gimbal(const char *ip_address, const int port) {
        // Create a UDP socket
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd_ < 0) {
            std::cout << "Error, failed create socket" << std::endl;
            throw std::runtime_error("Failed to create socket");
        }
    
        // Set up the server address
        server_addr_.sin_family = AF_INET;
        server_addr_.sin_addr.s_addr = inet_addr(ip_address);
        server_addr_.sin_port = htons(port);
        int sock_flags = fcntl(sockfd_,F_GETFL, 0);
        fcntl(sockfd_, F_SETFL, sock_flags | O_NONBLOCK);
    
        live = true;
        gimbal_attitude_thread = std::thread([this] { gimbal_attitude_loop(live); });
        gimbal_info_thread = std::thread([this] { gimbal_info_loop(live); });
        receive_message_thread = std::thread([this] { receive_message_loop(live); });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        SIYI_Gimbal::request_hardware_id();
    }
    
    SIYIUnix_Gimbal::~SIYIUnix_Gimbal() {
        // Close the socket
        close(sockfd_);
    
        live = false;
        gimbal_attitude_thread.join();
        gimbal_info_thread.join();
        receive_message_thread.join();
    }
    
    bool SIYIUnix_Gimbal::send_message(const uint8_t *message, const int length) const {
        // Send a response to the client
        ssize_t send_len = sendto(sockfd_, message, length, 0, (struct sockaddr *) &server_addr_, sizeof(server_addr_));
        if (send_len < 0) {
            std::cerr << "Error, failed to send message" << std::endl;
            return false;
        }
        return true;
    }
    
    void SIYIUnix_Gimbal::receive_message_loop(bool &connected) {
        uint8_t buff[BUFFER_SIZE];
        while (connected) {
            int bytes = int(recvfrom(sockfd_, buff, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr_, &server_addr_len));
    
            if(bytes < 0) {
            } else {
                // Go through the buffer
                if( memcmp(&HEADER,buff,2) || bytes < MINIMUM_DATA_LENGTH ) {
                    continue;
                }
                msg.decode_msg(buff);
                // Check if there is enough data (including payload)
                if (bytes >= MINIMUM_DATA_LENGTH + msg.get_data_len() ) {
                    uint8_t cmd_id = msg.get_cmd_id();
                    // Message parsing distribution in frequent use respective order
                    if (cmd_id == GIMB_ACQUIRE_GIMBAL_ATTITUDE) SIYI_Gimbal::parse_gimbal_attitude_msg();
                    else if (cmd_id == GIMB_ACQUIRE_GIMBAL_INFO) SIYI_Gimbal::parse_gimbal_info_msg();
                    else if (cmd_id == GIMB_MANUAL_ZOOM) SIYI_Gimbal::parse_manual_zoom_msg();
                    else if (cmd_id == ACQUIRE_FIRMWARE_VERSION) SIYI_Gimbal::parse_firmware_version_msg();
                    else if (cmd_id == GIMB_ACQUIRE_HARDWARE_ID) SIYI_Gimbal::parse_hardware_id_msg();
                    else if (cmd_id == GIMB_FUNCTION_FEEDBACK_INFO) SIYI_Gimbal::parse_function_feedback_msg();
                    else if (cmd_id == GIMB_ROTATION) SIYI_Gimbal::parse_gimbal_speed_msg();
                    else if (cmd_id == GIMB_CONTROL_ANGLE) SIYI_Gimbal::parse_gimbal_angles_msg();
                    else if (cmd_id == GIMB_AUTOFOCUS) SIYI_Gimbal::parse_autofocus_msg();
                    else if (cmd_id == GIMB_MANUAL_FOCUS) SIYI_Gimbal::parse_manual_focus_msg();
                    else if (cmd_id == GIMB_CENTER) SIYI_Gimbal::parse_gimbal_center_msg();
                    else if (cmd_id == GIMB_ABSOLUTE_ZOOM) SIYI_Gimbal::parse_absolute_zoom_msg();
                    else if (cmd_id == GIMB_ACQUIRE_MAX_ZOOM) SIYI_Gimbal::parse_maximum_zoom_msg();
                } else {
                }
            }
    
            // Convert received bytes to hex string in uppercase
        }
    }
    
    void SIYIUnix_Gimbal::gimbal_attitude_loop(bool &connected) {
        while (connected) {
            SIYI_Gimbal::request_gimbal_attitude();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // set frequency to 100 Hz
        }
    }
    
    void SIYIUnix_Gimbal::gimbal_info_loop(bool &connected) {
        while (connected) {
            SIYI_Gimbal::request_firmware_version();
            SIYI_Gimbal::request_gimbal_info();
            std::this_thread::sleep_for(std::chrono::seconds(1));  // set frequency to 1 Hz
        }
    }
} //namespace SIYI

