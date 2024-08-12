#pragma once
#include "sdk.hpp"
#include <thread>
#include <arpa/inet.h>

namespace SIYI {
    class SIYIUnix_Gimbal : public SIYI_Gimbal {
    public:
        SIYIUnix_Gimbal(const char *ip_address = "192.168.144.25", int port = 37260);
        virtual ~SIYIUnix_Gimbal() override;
    private:
        virtual bool send_message(const uint8_t *message, const int length) const override;
    
        bool live = false;
        std::thread receive_message_thread;
        std::thread gimbal_attitude_thread;
        std::thread gimbal_info_thread;
        void receive_message_loop(bool &connected);
        void gimbal_attitude_loop(bool &connected);
        void gimbal_info_loop(bool &connected);
    
        int sockfd_;
        struct sockaddr_in server_addr_{};
        socklen_t server_addr_len = sizeof(server_addr_);
    };
} //namespace SIYI

