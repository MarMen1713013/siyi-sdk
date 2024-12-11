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
    int8_t focus = 0;
    bool autofocus = false;
    

    printf("\x1B[?25l");
    gimball.request_follow_mode();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while(!stop) {
        file.read((char*)&event, sizeof(event));
        if(event.type == EV_KEY) {
            if(event.value == 1) switch(event.code) {
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
                    yaw -= 5.0;
                    break;
                case KEY_S:
                    if(pitch > -85) pitch -= 5;
                    break;
                case KEY_A:
                    yaw += 5.0;
                    break;
                case KEY_R:
                    integer = 1;
                    fractional = 0;
                    yaw = 0;
                    pitch = 0;
                    autofocus = true;
                    break;
                default:
                    printf("CODE: %d\n", event.code);
                    break;
            } else if(event.value == 2) switch(event.code) { //pressed more
                case KEY_LEFT:
                    focus = -1;
                    break;
                case KEY_RIGHT:
                    focus = 1;
                    break;
                default: break;
            } else if(event.value == 0) switch(event.code) { //release
                case KEY_LEFT:
                case KEY_RIGHT:
                    focus = 0;
                    break;
                default: break;
            }
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
            //act
            gimball.set_absolute_zoom(integer, fractional);
            gimball.set_gimbal_angles(yaw,pitch);
            if(!autofocus) switch(focus) {
                case -1:
                    gimball.request_focus_far();
                    break;
                case 1:
                    gimball.request_focus_close();
                    break;
                case 0:
                    gimball.request_focus_halt();
                    break;
                default: break;
            } else {
                gimball.request_autofocus();
            }
        }
    }
    printf("\x1B[?25h");
    return 0;
}

