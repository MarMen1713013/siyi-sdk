#include <chrono>
#include <iostream>
#include <siyi-sdk.h>
#include <csignal>
#include <atomic>
#include <functional>
#include <thread>

using namespace SIYI;

std::function<void(int)> sigout;
void sigout_wr(int signum) {
    sigout(signum);
    printf("Closing...\n");
    printf("\033[?25h");
}

int main( int argc, char *argv[] ) {
    signal(SIGINT,sigout_wr);
    auto tracker = new SIYIUnix_AI();
    std::atomic<bool> stop_flag;
    stop_flag = false;

    sigout = [&](int signum){
        stop_flag = true;
        std::cout << "\nReceived signal " << signum << std::endl;
    };
    while(!stop_flag) {
        auto tmp = tracker->get_tracking_state();
        std::cout << "\nAI mode: " << (std::get<0>(tmp).ai_tracking ? "ON" : "OFF") << std::endl;
        std::cout << "Target present: " << (std::get<0>(tmp).target_present ? "YES" : "NO" ) << std::endl;
        std::cout << "Requested coordinates: [x: " << (std::get<0>(tmp).x_req) << ", y: " << (std::get<0>(tmp).y_req) << "]" << std::endl;;
        std::cout << "Tracking status: ";
        switch(std::get<1>(tmp)) {
            #define CASE(x) case x: std::cout << #x << std::endl; break
            CASE(SIYIUnix_AI::FAILURE);
            CASE(SIYIUnix_AI::SUCCESS);
            CASE(SIYIUnix_AI::TRACKING_NOT_AI);
            CASE(SIYIUnix_AI::UNSUPPORTED_STREAM);
            default:
                break;
            #undef CASE
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    delete tracker;
    return 0;
}
