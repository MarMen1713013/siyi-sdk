#include <chrono>
#include <iostream>
#include <linux/input.h>
#include <fstream>
#include <stdint.h>
#include <siyi-sdk.h>
#include <thread>

int main(int argc, char *argv[]) {
    
    std::ifstream file("/dev/input/by-id/usb-Logitech_USB_Receiver-if02-event-mouse");
    if(!file.is_open()) {
        std::cerr << "Could not open event file" << std::endl;
        return 1;
    }

    struct input_event event;
    bool stop = false;
    SIYI::SIYIUnix_Gimbal gimball;
    int integer=1;
    int fractional=0;
    float yaw = 0;
    float pitch = 0;
    

    printf("\x1B[?25l");
    gimball.request_follow_mode();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while(!stop) {
        if(fractional >= 10) {
            if(integer < 180) {
                integer += 1;
                fractional -= 10;
            }
        } else if(fractional < 0) {
            if(integer > 1) {
                integer -= 1;
                fractional += 10;
            }
        }
        file.read((char*)&event, sizeof(event));
        if(event.type == EV_KEY) {
            switch(event.code) {
                case KEY_ESC:
                    stop = true;
                    break;
                case KEY_UP:
                    fractional += 5;
                    break;
                case KEY_DOWN:
                    fractional -= 5;
                    break;
                case KEY_W:
                    if(pitch < 20.0) pitch += 5;
                    break;
                case KEY_D:
                    if(yaw > -270) yaw -= 5.0;
                    break;
                case KEY_S:
                    if(pitch > -85) pitch -= 5;
                    break;
                case KEY_A:
                    if(yaw < 270) yaw += 5.0;
                    break;
                default:
                    printf("CODE: %d\n", event.code);
                    break;
            }
            gimball.set_absolute_zoom(integer,fractional);
            gimball.set_gimbal_angles(yaw,pitch);
        }
    }
    printf("\x1B[?25h");
    return 0;
}

