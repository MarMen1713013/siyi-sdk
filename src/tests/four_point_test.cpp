#include <chrono>
#include <iostream>
#include <siyi-sdk.h> 
#include <csignal>
#include <functional>
#include <atomic>

std::function<void(int)> sigout;
void sigout_wr( int signum ) {
    sigout(signum);
    printf("Closing...\n");
    printf("\033[?25h");
}

int main(int argc, char *argv[] ) {
    std::cout << "Hello, world!" << std::endl;
    auto camera = new SIYIUnixCamera("192.168.144.25",37260);
    std::atomic<bool> flag;
    flag = true;

    sigout = [&](int s) {
        flag = false;
        printf("flag to false!\n");
    };
    signal(SIGINT,sigout_wr);

    std::atomic<int> index;
    index = 0;
    float angles[4][2] = {
        {20.0,-10.0},
        {20.0,10.0},
        {-20.0,10.0},
        {-20.0,-10.0}
    };
    camera->request_follow_mode();
    camera->print_message();
    std::cout << "Waiting for camera mode to be: FOLLOW" << std::endl;
    while(camera->get_motion_mode() != 1 && flag);
    flag = true;
    std::cout << "Running commands!" << std::endl;
    auto tmp = std::thread([&]{
        while(flag) {
            camera->set_gimbal_angles(angles[index][1], angles[index][0]);
            camera->print_message();
            index = (index + 1) % 4;
            std::this_thread::sleep_for(std::chrono::seconds(4));
        }         
    }); 
    while(flag) {
        //camera.request_gimbal_attitude(); //unnecessary. The constructor assign 3 threads for this
        auto att = camera->get_gimbal_attitude();
        auto roll = std::get<2>(att);
        auto pitch = std::get<1>(att);
        auto yaw = std::get<0>(att);
        printf("[R: %+10.2f,P: %+10.2f,Y: %+10.2f]\n",roll,pitch,yaw);
        usleep(200000);
    }
    tmp.join();
    flag = true;
    camera->request_fpv_mode();
    camera->print_message();
    std::cout << "Waiting for camera mode to be: FPV" << std::endl;
    while(camera->get_motion_mode() != 2 && flag);
    delete camera;
    return 0;
}
