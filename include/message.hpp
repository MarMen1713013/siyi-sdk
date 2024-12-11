#pragma once

#include <cstdint>
#include <sys/types.h>
#include <crc.h>
/**
 * General
 */
#define ACQUIRE_FIRMWARE_VERSION 0x01

/**
 * Gimball messages
 */
#define GIMB_ACQUIRE_HARDWARE_ID 0x02
#define GIMB_AUTOFOCUS 0x04
#define GIMB_MANUAL_ZOOM 0x05
#define GIMB_ABSOLUTE_ZOOM 0x0f
#define GIMB_ACQUIRE_MAX_ZOOM 0x16
#define GIMB_MANUAL_FOCUS 0x06
#define GIMB_ROTATION 0x07
#define GIMB_CENTER 0x08
#define GIMB_ACQUIRE_GIMBAL_INFO 0x0a
#define GIMB_FUNCTION_FEEDBACK_INFO 0x0b
#define GIMB_PHOTO_VIDEO 0x0c
#define GIMB_ACQUIRE_GIMBAL_ATTITUDE 0x0d
#define GIMB_CONTROL_ANGLE 0x0e

/**
 * AI tracking module
 */
#define AI_TCP_HEARTBEAT 0x00
#define AI_REQUEST_AI_MODE_SWITCH 0x03
#define AI_SET_AI_MODE_SWITCH 0x04
#define AI_REQUEST_AI_TRACKING_STATUS 0x05
#define AI_SET_AI_TRACKING_TARGET_BY_COORD 0x06

/**
 * MK32 control unit
 */
#define MK32CU_HARDWARE_ID 0x40
#define MK32CU_ACQUIRE_SYSTEM_SETTINGS 0x16
#define MK32CU_SYSTEM_SETTINGS 0x17
#define MK32CU_ACQUIRE_REMOTE_CONTROL_CHANNELS 0x42
#define MK32CU_ACQUIRE_RC_LINK_STATUS 0x43
#define MK32CU_ACQUIRE_FPV_LINK_STATUS 0x44

#define crc_red(x,y) crc16_T(SIYI_Message::s_table,x,y,0x0000,false,false,0x0000)
#define gen_tab(x) gen_lookup16(x,0x1021,false)

namespace SIYI {
    class SIYI_Message {
    public:
        explicit SIYI_Message();
        ~SIYI_Message();
        void decode_msg(const uint8_t *msg);
        uint16_t get_data_len() const;
        uint16_t get_seq() const;
        uint8_t get_cmd_id() const;
        const uint8_t *const get_data() const;
        void force_seq(int val) const;
    
        uint16_t get_send_data_len() const;
        
        /////////////////////////
        // MESSAGE DEFINITIONS //
        /////////////////////////
        
        const uint8_t *const firmware_version_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_hardware_id_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_autofocus_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_zoom_in_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_zoom_out_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_zoom_halt_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_absolute_zoom_msg(uint8_t *const, uint8_t integer, uint8_t fractional) const;
        
        const uint8_t *const gimbal_maximum_zoom_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_focus_far_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_focus_close_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_focus_halt_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_speed_msg(uint8_t *const, int8_t yaw_speed, int8_t pitch_speed) const;
        
        const uint8_t *const gimbal_center_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_info_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_lock_mode_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_follow_mode_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_fpv_mode_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_function_feedback_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_photo_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_record_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_attitude_msg(uint8_t *const) const;
        
        const uint8_t *const gimbal_angles_msg(uint8_t *const, float yaw, float pitch) const;
    
        const uint8_t *const ai_tcp_heartbeat(uint8_t *const) const;
    
        const uint8_t *const ai_request_ai_mode_switch(uint8_t *const) const;
    
        const uint8_t *const ai_set_ai_mode_switch(uint8_t *const, const bool) const;
    
        const uint8_t *const ai_request_ai_tracking_status(uint8_t *const) const;
    
        const uint8_t *const ai_set_ai_tracking_target_by_coord(uint8_t *const, bool, uint16_t, uint16_t) const;

        const uint8_t *const mk32cu_hardware_id_msg(uint8_t *const) const;

        const uint8_t *const mk32cu_acquire_system_settings(uint8_t *const) const;

        const uint8_t *const mk32cu_system_settings(uint8_t *const, uint8_t match, uint8_t baud_type, uint8_t joy_type) const;

        const uint8_t *const mk32cu_acquire_control_channel(uint8_t *const, uint8_t out_freq) const;

        const uint8_t *const mk32cu_acquire_rc_link_status(uint8_t *const) const;

        const uint8_t *const mk32cu_acquire_fpv_link_status(uint8_t *const) const;

    private:
        void encode_msg(uint8_t *const msg, const uint8_t cmd_id, const uint8_t *data, const uint16_t data_len) const;
        void increment_seq(int val) const;
        static size_t m_instances;
        static uint16_t s_table[256];
        static const int MINIMUM_DATA_LENGTH = 10;
    
        static const uint16_t HEADER = 0x6655;
        uint8_t m_ctrl = 0x01;
        uint16_t m_data_len;
        mutable uint16_t m_seq = 0;
        uint8_t m_cmd_id = 0x00;
        uint8_t m_data[64];
    
        mutable uint16_t m_send_data_len;
    };
} //namespace SIYI

