#include "remote.hpp"
#include <iostream>
#include <fcntl.h>
#include <cstring>

namespace SIYI {
    SIYIUnix_Remote::SIYIUnix_Remote(const char *ip_address, const int port) {
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
        receive_message_thread = std::thread([this] { receive_message_loop(live); });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        SIYI_Remote::request_hardware_id();
    }
    
    SIYIUnix_Remote::~SIYIUnix_Remote() {
        // Close the socket
        close(sockfd_);
    
        live = false;
        receive_message_thread.join();
    }
    
    bool SIYIUnix_Remote::send_message(const uint8_t *message, const int length) const {
        // Send a response to the client
        ssize_t send_len = sendto(sockfd_, message, length, 0, (struct sockaddr *) &server_addr_, sizeof(server_addr_));
        if (send_len < 0) {
            std::cerr << "Error, failed to send message" << std::endl;
            return false;
        }
        return true;
    }
    
    void SIYIUnix_Remote::receive_message_loop(bool &connected) {
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
                    if (cmd_id == MK32CU_ACQUIRE_SYSTEM_SETTINGS) SIYI_Remote::parse_req_system_settings();
                    else if (cmd_id == MK32CU_SYSTEM_SETTINGS) SIYI_Remote::parse_set_system_settings();
                    else if (cmd_id == MK32CU_ACQUIRE_REMOTE_CONTROL_CHANNELS) SIYI_Remote::parse_data_channel();
                    else if (cmd_id == MK32CU_ACQUIRE_RC_LINK_STATUS) SIYI_Remote::parse_rc_link_status();
                    else if (cmd_id == MK32CU_ACQUIRE_FPV_LINK_STATUS) SIYI_Remote::parse_fpv_link_status();
                } else {
                }
            }
    
            // Convert received bytes to hex string in uppercase
        }
    }
} //namespace SIYI

