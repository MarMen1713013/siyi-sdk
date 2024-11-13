#pragma once
#include "sdk.hpp"
#include <thread>
#include <arpa/inet.h>

namespace SIYI {
    class SIYIUnix_Remote : public SIYI_Remote {
    public:
        SIYIUnix_Remote(const char *ip_address = "192.168.144.25", int port = 37260);
        virtual ~SIYIUnix_Remote() override;
    private:
        virtual bool send_message(const uint8_t *message, const int length) const override;
    
        bool live = false;
        std::thread receive_message_thread;
        void receive_message_loop(bool &connected);
    
        int sockfd_;
        struct sockaddr_in server_addr_{};
        socklen_t server_addr_len = sizeof(server_addr_);
    };
} //namespace SIYI

