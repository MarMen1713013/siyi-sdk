#include "message.hpp"
#include <iostream>
#include <cstring>

namespace SIYI {
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
    
    const uint8_t *const SIYI_Message::gimbal_hardware_id_msg(uint8_t *const msg) const {
        encode_msg(msg,GIMB_ACQUIRE_HARDWARE_ID,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_autofocus_msg(uint8_t *const msg) const {
        uint8_t data[5]; memset(data,0,sizeof(data));
        data[0] = 1;
        encode_msg(msg,GIMB_AUTOFOCUS,data,5);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_zoom_in_msg(uint8_t *const msg) const {
        uint8_t data = 1;
        encode_msg(msg,GIMB_MANUAL_ZOOM,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_zoom_out_msg(uint8_t *const msg) const {
        uint8_t data = -1;
        encode_msg(msg,GIMB_MANUAL_ZOOM,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_zoom_halt_msg(uint8_t *const msg) const {
        uint8_t data = 0;
        encode_msg(msg,GIMB_MANUAL_ZOOM,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_absolute_zoom_msg(uint8_t *const msg, uint8_t integer, uint8_t fractional) const {
        uint8_t data[2];
        data[0] = integer;
        data[1] = fractional;
        encode_msg(msg,GIMB_ABSOLUTE_ZOOM,data,2);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_maximum_zoom_msg(uint8_t *const msg) const {
        encode_msg(msg,GIMB_ACQUIRE_MAX_ZOOM,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_focus_far_msg(uint8_t *const msg) const {
        uint8_t data = 1;
        encode_msg(msg,GIMB_MANUAL_FOCUS,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_focus_close_msg(uint8_t *const msg) const {
        uint8_t data = -1;
        encode_msg(msg,GIMB_MANUAL_FOCUS,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_focus_halt_msg(uint8_t *const msg) const {
        uint8_t data = 0;
        encode_msg(msg,GIMB_MANUAL_FOCUS,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_speed_msg(uint8_t *const msg, int8_t yaw_speed, int8_t pitch_speed) const {
        int8_t data[2];
        if(abs(yaw_speed) > 100) yaw_speed = yaw_speed > 0 ? 100 : -100;
        if(abs(pitch_speed) > 100) pitch_speed = pitch_speed > 0 ? 100 : -100;
        data[0] = yaw_speed;
        data[1] = pitch_speed;
        encode_msg(msg,GIMB_ROTATION,(uint8_t*)data,2);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_center_msg(uint8_t *const msg) const {
        uint8_t data = 1;
        encode_msg(msg,GIMB_CENTER,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_info_msg(uint8_t *const msg) const {
        encode_msg(msg,GIMB_ACQUIRE_GIMBAL_INFO,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_lock_mode_msg(uint8_t *const msg) const {
        uint8_t data = 3;
        encode_msg(msg,GIMB_PHOTO_VIDEO,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_follow_mode_msg(uint8_t *const msg) const {
        uint8_t data = 4;
        encode_msg(msg,GIMB_PHOTO_VIDEO,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_fpv_mode_msg(uint8_t *const msg) const {
        uint8_t data = 5;
        encode_msg(msg,GIMB_PHOTO_VIDEO,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_function_feedback_msg(uint8_t *const msg) const {
        encode_msg(msg,GIMB_FUNCTION_FEEDBACK_INFO,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_photo_msg(uint8_t *const msg) const {
        uint8_t data = 0;
        encode_msg(msg,GIMB_PHOTO_VIDEO,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_record_msg(uint8_t *const msg) const {
        uint8_t data = 2;
        encode_msg(msg,GIMB_PHOTO_VIDEO,&data,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_attitude_msg(uint8_t *const msg) const {
        encode_msg(msg,GIMB_ACQUIRE_GIMBAL_ATTITUDE,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::gimbal_angles_msg(uint8_t *const msg, float yaw, float pitch) const {
        // Check if yaw angle exceeded limit and convert it to hex string
        int16_t data[2];
        data[0] = yaw * 10;
        data[1] = pitch * 10;
        encode_msg(msg,GIMB_CONTROL_ANGLE,(uint8_t*)data,sizeof(data));
        return msg;
    }
    
    const uint8_t *const SIYI_Message::ai_tcp_heartbeat(uint8_t *const msg) const {
        encode_msg(msg,AI_TCP_HEARTBEAT,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::ai_request_ai_mode_switch(uint8_t *const msg) const {
        encode_msg(msg,AI_REQUEST_AI_TRACKING_STATUS,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::ai_set_ai_mode_switch(uint8_t *const msg, const bool in) const {
        uint8_t tmp = in;
        encode_msg(msg,AI_SET_AI_MODE_SWITCH,&tmp,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::ai_request_ai_tracking_status(uint8_t *const msg) const {
        encode_msg(msg,AI_REQUEST_AI_TRACKING_STATUS,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::ai_set_ai_tracking_target_by_coord(uint8_t *const msg, bool track, uint16_t touch_x, uint16_t touch_y) const {
        uint8_t data[5];
        data[0] = track;
        memcpy(data+1,&touch_x,sizeof(uint16_t));
        memcpy(data+3,&touch_y,sizeof(uint16_t));
        encode_msg(msg,AI_SET_AI_TRACKING_TARGET_BY_COORD,data,5);
        return msg;
    }
    const uint8_t *const SIYI_Message::mk32cu_hardware_id_msg(uint8_t *const msg) const {
        encode_msg(msg,MK32CU_HARDWARE_ID,nullptr,0);
        return msg;
    }
    const uint8_t *const SIYI_Message::mk32cu_acquire_system_settings(uint8_t *const msg) const {
        encode_msg(msg,MK32CU_ACQUIRE_SYSTEM_SETTINGS,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::mk32cu_system_settings(uint8_t *const msg, uint8_t match, uint8_t baud_type, uint8_t joy_type) const {
        uint8_t data[4];
        data[0] = match;
        data[1] = baud_type;
        data[2] = joy_type;
        data[3] = 0;
        encode_msg(msg,MK32CU_SYSTEM_SETTINGS,data,4);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::mk32cu_acquire_control_channel(uint8_t *const msg, uint8_t out_freq) const {
        encode_msg(msg,MK32CU_ACQUIRE_REMOTE_CONTROL_CHANNELS,&out_freq,1);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::mk32cu_acquire_rc_link_status(uint8_t *const msg) const {
        encode_msg(msg,MK32CU_ACQUIRE_RC_LINK_STATUS,nullptr,0);
        return msg;
    }
    
    const uint8_t *const SIYI_Message::mk32cu_acquire_fpv_link_status(uint8_t *const msg) const {
        encode_msg(msg,MK32CU_ACQUIRE_FPV_LINK_STATUS,nullptr,0);
        return msg;
    }
} //namespace SIYI

