#include <chrono>
#include <siyi-sdk.h>
#include <csignal>
#include <thread>
#include <stdio.h>

using namespace SIYI;

int main( int argc, char *argv[] ) {
    SIYI::SIYIUnix_Remote remote("192.168.144.12",19856);
    remote.request_hardware_id();

    //std::this_thread::sleep_for(std::chrono::seconds(1));
    remote.request_data_channel(SIYI_Remote::FREQ_10_HZ);

    //std::this_thread::sleep_for(std::chrono::seconds(1));
    remote.request_data_channel(SIYI_Remote::FREQ_10_HZ);

    //std::this_thread::sleep_for(std::chrono::seconds(1));
    remote.request_data_channel(SIYI_Remote::FREQ_10_HZ);
    while(1) {
        auto chs = remote.get_channels();
        auto hd_id = remote.get_hardware_id();
        printf("\x1B[0KChannels:\n");
        printf("\x1B[0KDevice ID: %s\n",hd_id.id);
        for(int j = 0; j < 16; ++j) {
            printf("\x1B[0K\tch[%d]: %d\n", j, chs[j]);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(17));
        remote.request_system_settings();
        auto set = remote.get_system_settings();
        printf("\x1B[0KBAUD: %d, JOY: %d, MATCH: %d, BAT: %f\n",set.baud,set.joy,set.match,set.v_bat);
        remote.request_rc_link_status();
        auto rc_link = remote.get_rc_link_status();
        printf("\x1B[0KUP:%d,DN:%d, freq: %d\n",rc_link.data_up,rc_link.data_down,rc_link.freq);
        remote.request_fpv_link_status();
        auto fpv = remote.get_fpv_link_status();
        printf("\x1B[0KPWR: %ddBm\n",fpv.rssi);
        remote.request_hardware_id();
        printf("\x1B[21F");
    }
    return 0;
}
