#include "message.hpp"
#include <iostream>
#include <cstring>

size_t SIYI_Message::m_instances = 0;
uint16_t SIYI_Message::s_table[256];

SIYI_Message::SIYI_Message() {
    if(m_instances == 0)
        gen_tab(s_table);
    m_instances += 1;
}
SIYI_Message::~SIYI_Message() {
    if(m_instances != 0) 
        m_instances -= 1;
}

void SIYI_Message::increment_seq(int val) const {
    if (val < 0 || val > 65535) {
        m_seq = 0;
    } else m_seq = val + 1;
}

void SIYI_Message::decode_msg(const uint8_t *msg) {
    memcpy(&m_data_len,msg+3,sizeof(uint16_t));

    uint16_t crc_xmodem = crc_red((uint8_t*)msg,m_data_len+8); //last 2 bytes are crc
    uint16_t crc_received;
    memcpy(&crc_received,msg+8+m_data_len,sizeof(uint16_t));
    // Perform CRC16 checkout
    if (crc_xmodem != crc_received) {
        std::cout << "Warning, CRC16 error during message decoding" << std::endl;
        return;
    }

    // Get sequence
    memcpy(&m_seq,msg+5,sizeof(uint16_t));
    // Get command ID
    m_cmd_id = msg[7];

    // Get data
    if (m_data_len > 0)
        memcpy(m_data,msg+8,m_data_len);
}

uint16_t SIYI_Message::get_data_len() const {
    return m_data_len;
}

uint16_t SIYI_Message::get_seq() const {
    return m_seq;
}

uint8_t SIYI_Message::get_cmd_id() const {
    return m_cmd_id;
}

const uint8_t *const SIYI_Message::get_data() const {
    return m_data;
}

void SIYI_Message::force_seq(int val) const {
    m_seq = val;
}

uint16_t SIYI_Message::get_send_data_len() const {
    return m_send_data_len;
}

void SIYI_Message::encode_msg(uint8_t *const msg, const uint8_t cmd_id, const uint8_t *data, const uint16_t data_len) const {
    if(data_len < 0) {
        std::cerr << "Length can't be smaller than 0" << std::endl;
        return;
    }
    increment_seq(m_seq);
    memcpy(msg,&HEADER,sizeof(uint16_t));
    msg[2] = m_ctrl;
    memcpy(msg+3,&data_len,sizeof(uint16_t));
    memcpy(msg+5,&m_seq,sizeof(uint16_t));
    msg[7] = cmd_id;
    if(data_len > 0) 
        memcpy(msg+8,data,data_len);
    uint16_t crc_xmodem = crc_red(msg,8+data_len);
    memcpy(msg+8+data_len,&crc_xmodem,sizeof(uint16_t));
    m_send_data_len = 10 + data_len;
}

/////////////////////////
// MESSAGE DEFINITIONS //
/////////////////////////

const uint8_t *const SIYI_Message::firmware_version_msg(uint8_t *const msg) const {
    encode_msg(msg,ACQUIRE_FIRMWARE_VERSION,nullptr,0);
    return msg;
}

const uint8_t *const SIYI_Message::hardware_id_msg(uint8_t *const msg) const {
    encode_msg(msg,ACQUIRE_HARDWARE_ID,nullptr,0);
    return msg;
}

const uint8_t *const SIYI_Message::autofocus_msg(uint8_t *const msg) const {
    uint8_t data[5]; memset(data,0,sizeof(data));
    data[0] = 1;
    encode_msg(msg,AUTOFOCUS,data,5);
    return msg;
}

const uint8_t *const SIYI_Message::zoom_in_msg(uint8_t *const msg) const {
    uint8_t data = 1;
    encode_msg(msg,MANUAL_ZOOM,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::zoom_out_msg(uint8_t *const msg) const {
    uint8_t data = -1;
    encode_msg(msg,MANUAL_ZOOM,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::zoom_halt_msg(uint8_t *const msg) const {
    uint8_t data = 0;
    encode_msg(msg,MANUAL_ZOOM,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::absolute_zoom_msg(uint8_t *const msg, uint8_t integer, uint8_t fractional) const {
    uint8_t data[2];
    data[0] = integer;
    data[1] = fractional;
    encode_msg(msg,ABSOLUTE_ZOOM,data,2);
    return msg;
}

const uint8_t *const SIYI_Message::maximum_zoom_msg(uint8_t *const msg) const {
    encode_msg(msg,ACQUIRE_MAX_ZOOM,nullptr,0);
    return msg;
}

const uint8_t *const SIYI_Message::focus_far_msg(uint8_t *const msg) const {
    uint8_t data = 1;
    encode_msg(msg,MANUAL_FOCUS,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::focus_close_msg(uint8_t *const msg) const {
    uint8_t data = -1;
    encode_msg(msg,MANUAL_FOCUS,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::focus_halt_msg(uint8_t *const msg) const {
    uint8_t data = 0;
    encode_msg(msg,MANUAL_FOCUS,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::gimbal_speed_msg(uint8_t *const msg, int8_t yaw_speed, int8_t pitch_speed) const {
    int8_t data[2];
    if(abs(yaw_speed) > 100) yaw_speed = yaw_speed > 0 ? 100 : -100;
    if(abs(pitch_speed) > 100) pitch_speed = pitch_speed > 0 ? 100 : -100;
    data[0] = yaw_speed;
    data[1] = pitch_speed;
    encode_msg(msg,GIMBAL_ROTATION,(uint8_t*)data,2);
    return msg;
}

const uint8_t *const SIYI_Message::gimbal_center_msg(uint8_t *const msg) const {
    uint8_t data = 1;
    encode_msg(msg,CENTER,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::gimbal_info_msg(uint8_t *const msg) const {
    encode_msg(msg,ACQUIRE_GIMBAL_INFO,nullptr,0);
    return msg;
}

const uint8_t *const SIYI_Message::lock_mode_msg(uint8_t *const msg) const {
    uint8_t data = 3;
    encode_msg(msg,PHOTO_VIDEO,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::follow_mode_msg(uint8_t *const msg) const {
    uint8_t data = 4;
    encode_msg(msg,PHOTO_VIDEO,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::fpv_mode_msg(uint8_t *const msg) const {
    uint8_t data = 5;
    encode_msg(msg,PHOTO_VIDEO,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::function_feedback_msg(uint8_t *const msg) const {
    encode_msg(msg,FUNCTION_FEEDBACK_INFO,nullptr,0);
    return msg;
}

const uint8_t *const SIYI_Message::photo_msg(uint8_t *const msg) const {
    uint8_t data = 0;
    encode_msg(msg,PHOTO_VIDEO,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::record_msg(uint8_t *const msg) const {
    uint8_t data = 2;
    encode_msg(msg,PHOTO_VIDEO,&data,1);
    return msg;
}

const uint8_t *const SIYI_Message::gimbal_attitude_msg(uint8_t *const msg) const {
    encode_msg(msg,ACQUIRE_GIMBAL_ATTITUDE,nullptr,0);
    return msg;
}

const uint8_t *const SIYI_Message::gimbal_angles_msg(uint8_t *const msg, float yaw, float pitch) const {
    // Check if yaw angle exceeded limit and convert it to hex string
    int16_t data[2];
    data[0] = yaw > 135 ? 1350 : yaw < -135 ? -1350 : yaw * 10;
    data[1] = pitch > 25 ? 250 : pitch < -90 ? -900 : pitch * 10;
    encode_msg(msg,CONTROL_ANGLE,(uint8_t*)data,sizeof(data));
    return msg;
}
