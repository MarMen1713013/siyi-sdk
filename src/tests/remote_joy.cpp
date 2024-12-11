#include <chrono>
#include <siyi-sdk.h>
#include <csignal>
#include <thread>
#include <stdio.h>

using namespace SIYI;

int main( int argc, char *argv[] ) {
    SIYI::SIYIUnix_Remote remote("192.168.144.20",19856);
    remote.request_hardware_id();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    remote.request_data_channel(SIYI_Remote::FREQ_10_HZ);
    for(int i = 0; i < 40; ++i) {
        auto chs = remote.get_channels();
        printf("Channels:\n");
        for(int j = 0; j < 16; ++j) {
            printf("\tch[%d]: %d\n", j, chs[j]);
        }
        printf("\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
