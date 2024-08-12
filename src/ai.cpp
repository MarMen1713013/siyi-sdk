#include "ai.hpp"
#include <chrono>
#include <iostream>
#include <fcntl.h>
#include <cstring> 
#include <thread>

namespace SIYI {
    SIYIUnix_AI::SIYIUnix_AI(const char *ip_address, const int port) { // Create a UDP socket
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
        receive_message_thread = std::thread([this] { receive_message_loop(live); });
        heartbeat_thread = std::thread([this] { heartbeat_loop(live); });
        tracking_status_thread = std::thread([this] { tracking_status_loop(live); });
    
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    SIYIUnix_AI::~SIYIUnix_AI() {
        // Close the socket
        close(sockfd_);
    
        live = false;
        tracking_status_thread.join();
        heartbeat_thread.join();
        receive_message_thread.join();
    }
    
    bool SIYIUnix_AI::send_message(const uint8_t *message, const int length) const {
        // Send a response to the client
        ssize_t send_len = sendto(sockfd_, message, length, 0, (struct sockaddr *) &server_addr_, sizeof(server_addr_));
        if (send_len < 0) {
            std::cerr << "Error, failed to send message" << std::endl;
            return false;
        }
        return true;
    }
    
    void SIYIUnix_AI::receive_message_loop(bool& connected) {
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
                    if (cmd_id == AI_REQUEST_AI_MODE_SWITCH) SIYI_AI::parse_tracking_mode();
                    else if (cmd_id == AI_SET_AI_MODE_SWITCH) SIYI_AI::parse_tracking_mode();
                    else if (cmd_id == AI_REQUEST_AI_TRACKING_STATUS) SIYI_AI::parse_tracking_status();
                    else if (cmd_id == AI_SET_AI_TRACKING_TARGET_BY_COORD) SIYI_AI::parse_ai_tracking_target();
                    else if (cmd_id == ACQUIRE_FIRMWARE_VERSION) SIYI_AI::parse_firmware_version_msg();
                } else {
                }
            }
        }
    }
    
    void SIYIUnix_AI::heartbeat_loop(bool& connected) {
        while(connected) {
            SIYI_AI::send_heartbeat();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void SIYIUnix_AI::tracking_status_loop(bool& connected) {
        while(connected) {
            SIYI_AI::request_ai_tracking_status();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
} //namespace SIYI

