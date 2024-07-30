#pragma once

#include <cstdint>
#include <sys/types.h>
#include <crc.h>
#define ACQUIRE_FIRMWARE_VERSION 0x01
#define ACQUIRE_HARDWARE_ID 0x02
#define AUTOFOCUS 0x04
#define MANUAL_ZOOM 0x05
#define ABSOLUTE_ZOOM 0x0f
#define ACQUIRE_MAX_ZOOM 0x16
#define MANUAL_FOCUS 0x06
#define GIMBAL_ROTATION 0x07
#define CENTER 0x08
#define ACQUIRE_GIMBAL_INFO 0x0a
#define FUNCTION_FEEDBACK_INFO 0x0b
#define PHOTO_VIDEO 0x0c
#define ACQUIRE_GIMBAL_ATTITUDE 0x0d
#define CONTROL_ANGLE 0x0e

#define crc_red(x,y) crc16_T(SIYI_Message::s_table,x,y,0x0000,false,false,0x0000)
#define gen_tab(x) gen_lookup16(x,0x1021,false)

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
    
    const uint8_t *const hardware_id_msg(uint8_t *const) const;
    
    const uint8_t *const autofocus_msg(uint8_t *const) const;
    
    const uint8_t *const zoom_in_msg(uint8_t *const) const;
    
    const uint8_t *const zoom_out_msg(uint8_t *const) const;
    
    const uint8_t *const zoom_halt_msg(uint8_t *const) const;
    
    const uint8_t *const absolute_zoom_msg(uint8_t *const, uint8_t integer, uint8_t fractional) const;
    
    const uint8_t *const maximum_zoom_msg(uint8_t *const) const;
    
    const uint8_t *const focus_far_msg(uint8_t *const) const;
    
    const uint8_t *const focus_close_msg(uint8_t *const) const;
    
    const uint8_t *const focus_halt_msg(uint8_t *const) const;
    
    const uint8_t *const gimbal_speed_msg(uint8_t *const, int8_t yaw_speed, int8_t pitch_speed) const;
    
    const uint8_t *const gimbal_center_msg(uint8_t *const) const;
    
    const uint8_t *const gimbal_info_msg(uint8_t *const) const;
    
    const uint8_t *const lock_mode_msg(uint8_t *const) const;
    
    const uint8_t *const follow_mode_msg(uint8_t *const) const;
    
    const uint8_t *const fpv_mode_msg(uint8_t *const) const;
    
    const uint8_t *const function_feedback_msg(uint8_t *const) const;
    
    const uint8_t *const photo_msg(uint8_t *const) const;
    
    const uint8_t *const record_msg(uint8_t *const) const;
    
    const uint8_t *const gimbal_attitude_msg(uint8_t *const) const;
    
    const uint8_t *const gimbal_angles_msg(uint8_t *const, float yaw, float pitch) const;

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

