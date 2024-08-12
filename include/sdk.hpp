#pragma once

#include "message.hpp"
#include <tuple>
#include <unistd.h>

namespace SIYI {
    class SIYI_SDK {
    public:
        SIYI_SDK() = default;
        virtual ~SIYI_SDK();
        void print_message() const;
    
    protected:
        void print_size_err(uint8_t) const;
        virtual bool send_message(const uint8_t *message, const int length) const = 0;
    
        const int MINIMUM_DATA_LENGTH = 10;
        const int BUFFER_SIZE = 1024;
        const int16_t HEADER = 0x6655;
        uint8_t m_msg_buffer[74];
        SIYI_Message msg;
    };
    
    class SIYI_Gimbal : public SIYI_SDK {
    public:
    
        struct FirmwareVersionMsg {
            int seq = 0;
            uint32_t code_board_version;
            uint32_t gimbal_firmware_version;
            uint32_t zoom_firmware_version;
        };
    
        struct HardwareIDMsg {
            int seq = 0;
            uint8_t id;
        };
    
        struct AutofocusMsg {
            int seq = 0;
            bool success = false;
        };
    
        struct ManualZoomMsg {
            int seq = 0;
            float zoom_level = -1;
        };
    
        struct AbsoluteZoomMsg {
            int seq = 0;
            bool success = false;
        };
    
        struct MaxZoomMsg {
            int seq = 0;
            float max_level = 0.0;
        };
    
        struct ManualFocusMsg {
            int seq = 0;
            bool success = false;
        };
    
        struct GimbalSpeedMsg {
            int seq = 0;
            bool success = false;
        };
    
        struct CenterMsg {
            int seq = 0;
            bool success = false;
        };
    
        struct RecordingStateMsg {
            int seq = 0;
            int state = -1;
        };
    
        struct MountingDirectionMsg {
            int seq = 0;
            int direction = -1;
        };
    
        struct MotionModeMsg {
            int seq = 0;
            int mode = -1;
        };
    
        struct FunctionFeedbackMsg {
            int seq = 0;
            int info_type = -1;
        };
    
        struct GimbalAttitudeMsg {
            int seq = 0;
            float yaw = 0.0;
            float pitch = 0.0;
            float roll = 0.0;
            float yaw_speed = 0.0;
            float pitch_speed = 0.0;
            float roll_speed = 0.0;
        };
    
        struct GimbalAnglesMsg {
            int seq = 0;
            float yaw = 0.0;
            float pitch = 0.0;
            float roll = 0.0;
        };
    
        /////////////////////////////////
        //  REQUEST AND SET FUNCTIONS  //
        /////////////////////////////////
    
        bool request_firmware_version();
    
        bool request_hardware_id();
    
        bool request_autofocus();
    
        bool request_zoom_in();
    
        bool request_zoom_out();
    
        bool request_zoom_halt();
    
        bool set_absolute_zoom(int integer, int fractional);
    
        bool request_maximum_zoom();
    
        bool request_focus_far();
    
        bool request_focus_close();
    
        bool request_focus_halt();
    
        bool set_gimbal_speed(int yaw_speed, int pitch_speed);
    
        bool request_gimbal_center();
    
        bool request_gimbal_info();
    
        bool request_lock_mode();
    
        bool request_follow_mode();
    
        bool request_fpv_mode();
    
        bool request_function_feedback();
    
        bool request_photo();
    
        bool request_record();
    
        bool request_gimbal_attitude();
    
        bool set_gimbal_angles(float yaw, float pitch);
    
    
        ///////////////////////
        //  PARSE FUNCTIONS  //
        ///////////////////////
    
        void parse_firmware_version_msg();
    
        void parse_hardware_id_msg();
    
        void parse_autofocus_msg();
    
        void parse_manual_zoom_msg();
    
        void parse_absolute_zoom_msg();
    
        void parse_maximum_zoom_msg();
    
        void parse_manual_focus_msg();
    
        void parse_gimbal_speed_msg();
    
        void parse_gimbal_center_msg();
    
        void parse_gimbal_info_msg();
    
        void parse_function_feedback_msg();
    
        void parse_gimbal_attitude_msg();
    
        void parse_gimbal_angles_msg();
    
        /////////////////////
        //  GET FUNCTIONS  //
        /////////////////////
    
        [[nodiscard]] std::tuple<uint32_t, uint32_t, uint32_t> get_firmware_version() const;
    
        [[nodiscard]] uint8_t get_hardware_id() const;
    
        [[nodiscard]] float get_zoom_level() const;
    
        [[nodiscard]] float get_maximum_zoom() const;
    
        [[nodiscard]] int get_recording_state() const;
    
        [[nodiscard]] int get_motion_mode() const;
    
        [[nodiscard]] int get_mounting_direction() const;
    
        [[nodiscard]] int get_function_feedback() const;
    
        [[nodiscard]] std::tuple<float, float, float> get_gimbal_attitude() const;
    
        [[nodiscard]] std::tuple<float, float, float> get_gimbal_attitude_speed() const;
    
    protected:
    
        FirmwareVersionMsg firmware_version_msg;
        HardwareIDMsg hardware_id_msg;
        AutofocusMsg autofocus_msg;
        ManualZoomMsg manual_zoom_msg;
        MaxZoomMsg max_zoom_msg;
        AbsoluteZoomMsg absoluteZoom_msg;
        ManualFocusMsg manual_focus_msg;
        GimbalSpeedMsg gimbal_speed_msg;
        CenterMsg gimbal_center_msg;
        RecordingStateMsg recording_state_msg;
        MountingDirectionMsg mounting_direction_msg;
        MotionModeMsg motion_mode_msg;
        FunctionFeedbackMsg function_feedback_msg;
        GimbalAttitudeMsg gimbal_att_msg;
        GimbalAnglesMsg gimbal_angles_msg;
    };
    
    class SIYI_AI : public SIYI_SDK{
    public:
        struct FirmwareVersionMsg {
            int seq = 0;
            uint32_t code_board_version;
            uint32_t gimbal_firmware_version;
            uint32_t zoom_firmware_version;
        };
        struct AiTracking {
            bool target_present;
            bool ai_tracking;
            uint16_t x_req;
            uint16_t y_req;
        };
        enum AiTrackingState {
            FAILURE = 0,
            SUCCESS = 1,
            TRACKING_NOT_AI = 2,
            UNSUPPORTED_STREAM = 3
        };
        /////////////////////////////////
        //  REQUEST AND SET FUNCTIONS  //
        /////////////////////////////////
    
        bool send_heartbeat();
        bool request_firmware_version();
        bool request_ai_tracking_switch();
        bool request_ai_tracking_status();
    
        bool set_ai_tracking_mode(bool track);
        bool set_ai_tracking_target(const AiTracking trg);
    
        ///////////////////////
        //  PARSE FUNCTIONS  //
        ///////////////////////
    
        void parse_firmware_version_msg();
        void parse_tracking_mode();
        void parse_tracking_status();
        void parse_ai_tracking_target();
    
        /////////////////////
        //  GET FUNCTIONS  //
        /////////////////////
    
        std::tuple<uint32_t, uint32_t, uint32_t> get_firmware_version() const;
        std::tuple<AiTracking,AiTrackingState> get_tracking_state() const;
    protected:
        AiTracking m_tracking;
        AiTrackingState m_tracking_state;
        FirmwareVersionMsg m_firmware_version_msg;
    };
} //namespace SIYI

