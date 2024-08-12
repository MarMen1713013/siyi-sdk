#include "sdk.hpp"
#include <iomanip>
#include <iostream>
#include <cstring>
#include <tuple>

namespace SIYI {
    SIYI_SDK::~SIYI_SDK() {}
    void SIYI_SDK::print_message() const {
        int len = msg.get_send_data_len();
        for(int i = 0; i < len; ++i) {
            printf("%.2X ", m_msg_buffer[i]);
        }
        printf("\n");
    }
    void SIYI_SDK::print_size_err(uint8_t in) const {
        std::cerr << "Received less bytes than necessary from msg: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)in << std::endl; 
    }
    
    /////////////////////////////////
    //  REQUEST AND SET FUNCTIONS  //
    /////////////////////////////////
    
    bool SIYI_Gimbal::request_firmware_version() {
        msg.firmware_version_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_hardware_id() {
        msg.gimbal_hardware_id_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_autofocus() {
        msg.gimbal_autofocus_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    bool SIYI_Gimbal::request_zoom_in() {
        msg.gimbal_zoom_in_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_zoom_out() {
        msg.gimbal_zoom_out_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_zoom_halt() {
        msg.gimbal_zoom_halt_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::set_absolute_zoom(int integer, int fractional) {
        msg.gimbal_absolute_zoom_msg(m_msg_buffer,integer,fractional);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_maximum_zoom() {
        msg.gimbal_maximum_zoom_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_focus_far() {
        msg.gimbal_focus_far_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_focus_close() {
        msg.gimbal_focus_close_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_focus_halt() {
        msg.gimbal_focus_halt_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::set_gimbal_speed(int yaw_speed, int pitch_speed) {
        msg.gimbal_speed_msg(m_msg_buffer,yaw_speed,pitch_speed);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_gimbal_center() {
        msg.gimbal_center_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_gimbal_info() {
        msg.gimbal_info_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_lock_mode() {
        msg.gimbal_lock_mode_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_follow_mode() {
        msg.gimbal_follow_mode_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_fpv_mode() {
        msg.gimbal_fpv_mode_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_function_feedback() {
        msg.gimbal_function_feedback_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_photo() {
        msg.gimbal_photo_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_record() {
        msg.gimbal_record_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::request_gimbal_attitude() {
        msg.gimbal_attitude_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_Gimbal::set_gimbal_angles(float yaw, float pitch) {
        msg.gimbal_angles_msg(m_msg_buffer,yaw,pitch);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    ///////////////////////
    //  PARSE FUNCTIONS  //
    ///////////////////////
    
    void SIYI_Gimbal::parse_firmware_version_msg() {
        if(msg.get_data_len() >= 12) {
            auto tmp = msg.get_data();
            firmware_version_msg.seq = msg.get_seq();
            memcpy(&(firmware_version_msg.code_board_version),tmp,4);
            memcpy(&(firmware_version_msg.gimbal_firmware_version),tmp+4,4);
            memcpy(&(firmware_version_msg.zoom_firmware_version),tmp+8,4);
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_hardware_id_msg() {
        if(msg.get_data_len() >= 1) {
            hardware_id_msg.seq = msg.get_seq();
            hardware_id_msg.id = msg.get_data()[0];
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_autofocus_msg() {
        if(msg.get_data_len() >= 1) {
            autofocus_msg.seq = msg.get_seq();
            autofocus_msg.success = msg.get_data()[0] == 1;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_manual_zoom_msg() {
        if(msg.get_data_len() >= 2) {
            uint16_t tmp_zoom_lvl;
            memcpy(&tmp_zoom_lvl,msg.get_data(),sizeof(uint16_t));
    
            manual_zoom_msg.seq = msg.get_seq();
            manual_zoom_msg.zoom_level = tmp_zoom_lvl / 10.0f;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_absolute_zoom_msg() {
        if(msg.get_data_len() >= 1) {
            absoluteZoom_msg.seq = msg.get_seq();
            absoluteZoom_msg.success = msg.get_data()[0] == 1;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_maximum_zoom_msg() {
        if(msg.get_data_len() >= 2) {
            max_zoom_msg.seq = msg.get_seq();
            max_zoom_msg.max_level = ( msg.get_data()[0] * 10.0f ) + (float)( msg.get_data()[1] ) / 10.0f;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_manual_focus_msg() {
        if(msg.get_data_len() >= 1) {
            manual_focus_msg.seq = msg.get_seq();
            manual_focus_msg.success = msg.get_data()[0] == 1;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_gimbal_speed_msg() {
        if(msg.get_data_len() >= 1) {
            gimbal_speed_msg.seq = msg.get_seq();
            gimbal_speed_msg.success = msg.get_data()[0] == 1;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_gimbal_center_msg() {
        if(msg.get_data_len() >= 1) {
            gimbal_center_msg.seq = msg.get_seq();
            gimbal_center_msg.success = msg.get_data()[0] == 1;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_gimbal_info_msg() {
        if(msg.get_data_len() >= 6) { //Different from documentation. It should be 7 bytes long
            recording_state_msg.seq = msg.get_seq();
            mounting_direction_msg.seq = msg.get_seq();
            motion_mode_msg.seq = msg.get_seq();
    
            recording_state_msg.state = msg.get_data()[3];
            motion_mode_msg.mode = msg.get_data()[4];
            mounting_direction_msg.direction = msg.get_data()[5];
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_function_feedback_msg() {
        if(msg.get_data_len() >= 1) {
            function_feedback_msg.seq = msg.get_seq();
            function_feedback_msg.info_type = msg.get_data()[0];
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_gimbal_attitude_msg() {
        if(msg.get_data_len() >= 12) {
            gimbal_att_msg.seq = msg.get_seq();
    
            auto tmp = (int16_t*)(msg.get_data());
    
            gimbal_att_msg.yaw   = -float(tmp[0]) / 10.0f;
            gimbal_att_msg.pitch = float(tmp[1]) / 10.0f;
            gimbal_att_msg.roll  = float(tmp[2]) / 10.0f;
    
            gimbal_att_msg.yaw_speed   = -float(tmp[3]) / 10.0f;
            gimbal_att_msg.pitch_speed = float(tmp[4]) / 10.0f;
            gimbal_att_msg.roll_speed  = float(tmp[5]) / 10.0f;
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_Gimbal::parse_gimbal_angles_msg() {
        if(msg.get_data_len() >= 6) {
            gimbal_angles_msg.seq = msg.get_seq();
    
            auto tmp = (int16_t*)(msg.get_data());
    
            gimbal_angles_msg.yaw   = -float(tmp[0]) / 10.0f;
            gimbal_angles_msg.pitch = float(tmp[1]) / 10.0f;
            gimbal_angles_msg.roll  = float(tmp[2]) / 10.0f;
        } else print_size_err(msg.get_cmd_id());
    }
    
    /////////////////////
    //  GET FUNCTIONS  //
    /////////////////////
    
    std::tuple<uint32_t, uint32_t, uint32_t> SIYI_Gimbal::get_firmware_version() const {
        return std::make_tuple(
            firmware_version_msg.code_board_version,
            firmware_version_msg.gimbal_firmware_version,
            firmware_version_msg.zoom_firmware_version
        );
    }
    
    uint8_t SIYI_Gimbal::get_hardware_id() const {
        return hardware_id_msg.id;
    }
    
    float SIYI_Gimbal::get_zoom_level() const {
        return manual_zoom_msg.zoom_level;
    }
    
    float SIYI_Gimbal::get_maximum_zoom() const {
        return max_zoom_msg.max_level;
    }
    
    int SIYI_Gimbal::get_recording_state() const {
        // 0 - off, 1 - on, 2 - slot empty, 3 - data loss
        return recording_state_msg.state;
    }
    
    int SIYI_Gimbal::get_motion_mode() const {
        // 0 - lock, 1 - follow, 2 - FPV
        return motion_mode_msg.mode;
    }
    
    int SIYI_Gimbal::get_mounting_direction() const {
        // 0 - normal, 1 - upside
        return mounting_direction_msg.direction;
    }
    
    int SIYI_Gimbal::get_function_feedback() const {
        // 0 - successful, 1 - photo fail, 2 - HDR on, 3 - HDR off, 4 - record fail
        return function_feedback_msg.info_type;
    }
    
    std::tuple<float, float, float> SIYI_Gimbal::get_gimbal_attitude() const {
        // yaw, pitch, roll angles in degrees
        return std::make_tuple(gimbal_att_msg.yaw, gimbal_att_msg.pitch, gimbal_att_msg.roll);
    }
    
    std::tuple<float, float, float> SIYI_Gimbal::get_gimbal_attitude_speed() const {
        // yaw, pitch, roll speeds in degrees/second
        return std::make_tuple(gimbal_att_msg.yaw_speed, gimbal_att_msg.pitch_speed, gimbal_att_msg.roll_speed);
    }
    
    /////////////////////////////////
    //  REQUEST AND SET FUNCTIONS  //
    /////////////////////////////////
    bool SIYI_AI::send_heartbeat() {
        msg.ai_tcp_heartbeat(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_AI::request_firmware_version() {
        msg.firmware_version_msg(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_AI::request_ai_tracking_switch() {
        msg.ai_request_ai_mode_switch(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_AI::request_ai_tracking_status() {
        msg.ai_request_ai_tracking_status(m_msg_buffer);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_AI::set_ai_tracking_mode(bool track) {
        msg.ai_set_ai_mode_switch(m_msg_buffer,track);
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    bool SIYI_AI::set_ai_tracking_target(const AiTracking trg) {
        m_tracking.x_req = trg.x_req;
        m_tracking.y_req = trg.y_req;
        msg.ai_set_ai_tracking_target_by_coord(
            m_msg_buffer,
            trg.ai_tracking,
            trg.x_req,
            trg.y_req
        );
        int len = msg.get_send_data_len();
        return send_message(m_msg_buffer,len);
    }
    
    ///////////////////////
    //  PARSE FUNCTIONS  //
    ///////////////////////
    
    void SIYI_AI::parse_firmware_version_msg() {
        if(msg.get_data_len() >= 12) {
            auto tmp = msg.get_data();
            m_firmware_version_msg.seq = msg.get_seq();
            memcpy(&(m_firmware_version_msg.code_board_version),tmp,4);
            memcpy(&(m_firmware_version_msg.gimbal_firmware_version),tmp+4,4);
            memcpy(&(m_firmware_version_msg.zoom_firmware_version),tmp+8,4);
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_AI::parse_tracking_mode() {
        if(msg.get_data_len() >= 1) {
            m_tracking.ai_tracking = msg.get_data()[0];
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_AI::parse_tracking_status() {
        if(msg.get_data_len() >= 1) {
            m_tracking.target_present = msg.get_data()[0];
        } else print_size_err(msg.get_cmd_id());
    }
    
    void SIYI_AI::parse_ai_tracking_target() {
        if(msg.get_data_len() >= 1) {
            m_tracking_state = (AiTrackingState)(msg.get_data()[0]);
        } else print_size_err(msg.get_cmd_id());
    }
    
    /////////////////////
    //  GET FUNCTIONS  //
    /////////////////////
    
    std::tuple<uint32_t, uint32_t, uint32_t> SIYI_AI::get_firmware_version() const {
        return std::make_tuple(
            m_firmware_version_msg.code_board_version,
            m_firmware_version_msg.gimbal_firmware_version,
            m_firmware_version_msg.zoom_firmware_version
        );
    }
    
    std::tuple<SIYI_AI::AiTracking, SIYI_AI::AiTrackingState> SIYI_AI::get_tracking_state() const {
        return std::make_tuple(
            m_tracking,
            m_tracking_state
        );
    }
} //namespace SIYI

