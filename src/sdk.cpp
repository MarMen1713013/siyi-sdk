#include "sdk.hpp"
#include <iomanip>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <thread>

SIYI_SDK::~SIYI_SDK() {}
void SIYI_SDK::print_message() const {
    int len = msg.get_send_data_len();
    for(int i = 0; i < len; ++i) {
        printf("%.2X ", m_msg_buffer[i]);
    }
    printf("\n");
}

/////////////////////////////////
//  REQUEST AND SET FUNCTIONS  //
/////////////////////////////////

bool SIYI_SDK::request_firmware_version() {
    msg.firmware_version_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_hardware_id() {
    msg.hardware_id_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_autofocus() {
    msg.autofocus_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_zoom_in() {
    msg.zoom_in_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_zoom_out() {
    msg.zoom_out_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_zoom_halt() {
    msg.zoom_halt_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::set_absolute_zoom(int integer, int fractional) {
    msg.absolute_zoom_msg(m_msg_buffer,integer,fractional);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_maximum_zoom() {
    msg.maximum_zoom_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_focus_far() {
    msg.focus_far_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_focus_close() {
    msg.focus_close_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_focus_halt() {
    msg.focus_halt_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::set_gimbal_speed(int yaw_speed, int pitch_speed) {
    msg.gimbal_speed_msg(m_msg_buffer,yaw_speed,pitch_speed);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_gimbal_center() {
    msg.gimbal_center_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_gimbal_info() {
    msg.gimbal_info_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_lock_mode() {
    msg.lock_mode_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_follow_mode() {
    msg.follow_mode_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_fpv_mode() {
    msg.fpv_mode_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_function_feedback() {
    msg.function_feedback_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_photo() {
    msg.photo_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_record() {
    msg.record_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::request_gimbal_attitude() {
    msg.gimbal_attitude_msg(m_msg_buffer);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

bool SIYI_SDK::set_gimbal_angles(float yaw, float pitch) {
    msg.gimbal_angles_msg(m_msg_buffer,yaw,pitch);
    int len = msg.get_send_data_len();
    return send_message(m_msg_buffer,len);
}

///////////////////////
//  PARSE FUNCTIONS  //
///////////////////////
void SIYI_SDK::print_size_err(uint8_t in) const {
    std::cerr << "Received less bytes than necessary from msg: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)in << std::endl; 
}

void SIYI_SDK::parse_firmware_version_msg() {
    if(msg.get_data_len() >= 12) {
        auto tmp = msg.get_data();
        firmware_version_msg.seq = msg.get_seq();
        memcpy(&(firmware_version_msg.code_board_version),tmp,4);
        memcpy(&(firmware_version_msg.gimbal_firmware_version),tmp+4,4);
        memcpy(&(firmware_version_msg.zoom_firmware_version),tmp+8,4);
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_hardware_id_msg() {
    if(msg.get_data_len() >= 1) {
        hardware_id_msg.seq = msg.get_seq();
        hardware_id_msg.id = msg.get_data()[0];
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_autofocus_msg() {
    if(msg.get_data_len() >= 1) {
        autofocus_msg.seq = msg.get_seq();
        autofocus_msg.success = msg.get_data()[0] == 1;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_manual_zoom_msg() {
    if(msg.get_data_len() >= 2) {
        uint16_t tmp_zoom_lvl;
        memcpy(&tmp_zoom_lvl,msg.get_data(),sizeof(uint16_t));

        manual_zoom_msg.seq = msg.get_seq();
        manual_zoom_msg.zoom_level = tmp_zoom_lvl / 10.0f;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_absolute_zoom_msg() {
    if(msg.get_data_len() >= 1) {
        absoluteZoom_msg.seq = msg.get_seq();
        absoluteZoom_msg.success = msg.get_data()[0] == 1;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_maximum_zoom_msg() {
    if(msg.get_data_len() >= 2) {
        max_zoom_msg.seq = msg.get_seq();
        max_zoom_msg.max_level = ( msg.get_data()[0] * 10.0f ) + (float)( msg.get_data()[1] ) / 10.0f;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_manual_focus_msg() {
    if(msg.get_data_len() >= 1) {
        manual_focus_msg.seq = msg.get_seq();
        manual_focus_msg.success = msg.get_data()[0] == 1;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_gimbal_speed_msg() {
    if(msg.get_data_len() >= 1) {
        gimbal_speed_msg.seq = msg.get_seq();
        gimbal_speed_msg.success = msg.get_data()[0] == 1;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_gimbal_center_msg() {
    if(msg.get_data_len() >= 1) {
        gimbal_center_msg.seq = msg.get_seq();
        gimbal_center_msg.success = msg.get_data()[0] == 1;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_gimbal_info_msg() {
    if(msg.get_data_len() >= 7) {
        recording_state_msg.seq = msg.get_seq();
        mounting_direction_msg.seq = msg.get_seq();
        motion_mode_msg.seq = msg.get_seq();

        recording_state_msg.state = msg.get_data()[3];
        motion_mode_msg.mode = msg.get_data()[4];
        mounting_direction_msg.direction = msg.get_data()[5];
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_function_feedback_msg() {
    if(msg.get_data_len() >= 1) {
        function_feedback_msg.seq = msg.get_seq();
        function_feedback_msg.info_type = msg.get_data()[0];
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_gimbal_attitude_msg() {
    if(msg.get_data_len() >= 12) {
        gimbal_att_msg.seq = msg.get_seq();

        auto tmp = (int16_t*)(msg.get_data());

        gimbal_att_msg.yaw   = float(tmp[0]) / 10.0f;
        gimbal_att_msg.pitch = float(tmp[1]) / 10.0f;
        gimbal_att_msg.roll  = float(tmp[2]) / 10.0f;

        gimbal_att_msg.yaw_speed   = float(tmp[3]) / 10.0f;
        gimbal_att_msg.pitch_speed = float(tmp[4]) / 10.0f;
        gimbal_att_msg.roll_speed  = float(tmp[5]) / 10.0f;
    } else print_size_err(msg.get_cmd_id());
}

void SIYI_SDK::parse_gimbal_angles_msg() {
    if(msg.get_data_len() >= 6) {
        gimbal_angles_msg.seq = msg.get_seq();

        auto tmp = (int16_t*)(msg.get_data());

        gimbal_angles_msg.yaw   = float(tmp[0]) / 10.0f;
        gimbal_angles_msg.pitch = float(tmp[1]) / 10.0f;
        gimbal_angles_msg.roll  = float(tmp[2]) / 10.0f;
    } else print_size_err(msg.get_cmd_id());
}

/////////////////////
//  GET FUNCTIONS  //
/////////////////////

std::tuple<uint32_t, uint32_t, uint32_t> SIYI_SDK::get_firmware_version() const {
    return std::make_tuple(firmware_version_msg.code_board_version, firmware_version_msg.gimbal_firmware_version,
                           firmware_version_msg.zoom_firmware_version);
}

uint8_t SIYI_SDK::get_hardware_id() const {
    return hardware_id_msg.id;
}

float SIYI_SDK::get_zoom_level() const {
    return manual_zoom_msg.zoom_level;
}

float SIYI_SDK::get_maximum_zoom() const {
    return max_zoom_msg.max_level;
}

int SIYI_SDK::get_recording_state() const {
    // 0 - off, 1 - on, 2 - slot empty, 3 - data loss
    return recording_state_msg.state;
}

int SIYI_SDK::get_motion_mode() const {
    // 0 - lock, 1 - follow, 2 - FPV
    return motion_mode_msg.mode;
}

int SIYI_SDK::get_mounting_direction() const {
    // 0 - normal, 1 - upside
    return mounting_direction_msg.direction;
}

int SIYI_SDK::get_function_feedback() const {
    // 0 - successful, 1 - photo fail, 2 - HDR on, 3 - HDR off, 4 - record fail
    return function_feedback_msg.info_type;
}

std::tuple<float, float, float> SIYI_SDK::get_gimbal_attitude() const {
    // yaw, pitch, roll angles in degrees
    return std::make_tuple(gimbal_att_msg.yaw, gimbal_att_msg.pitch, gimbal_att_msg.roll);
}

std::tuple<float, float, float> SIYI_SDK::get_gimbal_attitude_speed() const {
    // yaw, pitch, roll speeds in degrees/second
    return std::make_tuple(gimbal_att_msg.yaw_speed, gimbal_att_msg.pitch_speed, gimbal_att_msg.roll_speed);
}

/////////////////////////////
//  CAMERA IMPLEMENTATION  //
/////////////////////////////

SIYIUnixCamera::SIYIUnixCamera(const char *ip_address, const int port) {
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
    gimbal_attitude_thread = std::thread([this] { gimbal_attitude_loop(live); });
    gimbal_info_thread = std::thread([this] { gimbal_info_loop(live); });
    receive_message_thread = std::thread([this] { receive_message_loop(live); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    SIYI_SDK::request_hardware_id();
}

SIYIUnixCamera::~SIYIUnixCamera() {
    // Close the socket
    close(sockfd_);

    live = false;
    gimbal_attitude_thread.join();
    gimbal_info_thread.join();
    receive_message_thread.join();
}

bool SIYIUnixCamera::send_message(const uint8_t *message, const int length) const {
    // Send a response to the client
    ssize_t send_len = sendto(sockfd_, message, length, 0, (struct sockaddr *) &server_addr_, sizeof(server_addr_));
    if (send_len < 0) {
        std::cerr << "Error, failed to send message" << std::endl;
        return false;
    }
    return true;
}

void SIYIUnixCamera::receive_message_loop(bool &connected) {
    uint8_t buff[BUFFER_SIZE];
    while (connected) {
        int bytes = int(recvfrom(sockfd_, buff, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr_, &server_addr_len));

        if(bytes < 0) {
        } else {
            // Go through the buffer
            if( memcmp(&HEADER,buff,2) ) {
                continue;
            }
            msg.decode_msg(buff);
            // Check if there is enough data (including payload)
            if (bytes >= MINIMUM_DATA_LENGTH + msg.get_data_len() ) {
                uint8_t cmd_id = msg.get_cmd_id();
                // Message parsing distribution in frequent use respective order
                if (cmd_id == ACQUIRE_GIMBAL_ATTITUDE) SIYI_SDK::parse_gimbal_attitude_msg();
                else if (cmd_id == ACQUIRE_GIMBAL_INFO) SIYI_SDK::parse_gimbal_info_msg();
                else if (cmd_id == MANUAL_ZOOM) SIYI_SDK::parse_manual_zoom_msg();
                else if (cmd_id == ACQUIRE_FIRMWARE_VERSION) SIYI_SDK::parse_firmware_version_msg();
                else if (cmd_id == ACQUIRE_HARDWARE_ID) SIYI_SDK::parse_hardware_id_msg();
                else if (cmd_id == FUNCTION_FEEDBACK_INFO) SIYI_SDK::parse_function_feedback_msg();
                else if (cmd_id == GIMBAL_ROTATION) SIYI_SDK::parse_gimbal_speed_msg();
                else if (cmd_id == CONTROL_ANGLE) SIYI_SDK::parse_gimbal_angles_msg();
                else if (cmd_id == AUTOFOCUS) SIYI_SDK::parse_autofocus_msg();
                else if (cmd_id == MANUAL_FOCUS) SIYI_SDK::parse_manual_focus_msg();
                else if (cmd_id == CENTER) SIYI_SDK::parse_gimbal_center_msg();
                else if (cmd_id == ABSOLUTE_ZOOM) SIYI_SDK::parse_absolute_zoom_msg();
                else if (cmd_id == ACQUIRE_MAX_ZOOM) SIYI_SDK::parse_maximum_zoom_msg();
            } else {
            }
        }

        // Convert received bytes to hex string in uppercase
    }
}

void SIYIUnixCamera::gimbal_attitude_loop(bool &connected) {
    while (connected) {
        SIYI_SDK::request_gimbal_attitude();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // set frequency to 100 Hz
    }
}

void SIYIUnixCamera::gimbal_info_loop(bool &connected) {
    while (connected) {
        SIYI_SDK::request_firmware_version();
        SIYI_SDK::request_gimbal_info();
        std::this_thread::sleep_for(std::chrono::seconds(1));  // set frequency to 1 Hz
    }
}
