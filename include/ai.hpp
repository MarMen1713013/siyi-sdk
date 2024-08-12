#pragma once
#include "sdk.hpp"
#include <thread>
#include <arpa/inet.h>

namespace SIYI {
    class SIYIUnix_AI : public SIYI_AI {
    public:
        SIYIUnix_AI(const char *ip_address = "192.168.144.60", int port = 37260);
        virtual ~SIYIUnix_AI() override;
    private:
        virtual bool send_message(const uint8_t *message, const int length) const override;
    
        bool live = false;
        std::thread receive_message_thread;
        std::thread heartbeat_thread;
        std::thread tracking_status_thread;
        void receive_message_loop(bool &connected);
        void heartbeat_loop(bool &connected);
        void tracking_status_loop(bool &connected);
    
        int sockfd_;
        struct sockaddr_in server_addr_{};
        socklen_t server_addr_len = sizeof(server_addr_);
    };
} //namespace SIYI

