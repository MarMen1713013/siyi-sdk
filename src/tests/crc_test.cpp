#include <siyi-sdk.h>
#include <iostream>

using namespace SIYI;

class TestSDK : public SIYI_Gimbal {
public:
    TestSDK() {
        msg.force_seq(-1);
    }
    virtual ~TestSDK() { }
private:
    virtual bool send_message(const uint8_t *message, const int length) const override {
        msg.force_seq(-1);
        for(int i = 0; i < length; ++i) {
            printf("%.2X ",message[i]);
        }
        printf("\n");
        return true;
    }
};

class TestAI : public SIYI_AI {
public:
    TestAI() {
        msg.force_seq(-1);
    }
    virtual ~TestAI() { }
    virtual bool send_message(const uint8_t *message, const int length) const override {
        msg.force_seq(-1);
        for(int i = 0; i < length; ++i) {
            printf("%.2X ",message[i]);
        }
        printf("\n");
        return true;
    }
};

int main( int argc, char *argv[] ) {
    TestSDK test;
    auto tracker = new TestAI;

    std::cout << "////////////////" << std::endl;
    std::cout << "//   GIMBAL   //" << std::endl;
    std::cout << "////////////////\n" << std::endl;
    std::cout << "Zoom 1:" << std::endl;
    auto fuffa = test.request_zoom_in();
    std::cout << std::endl;

    std::cout << "Zoom -1:" << std::endl;
    fuffa = test.request_zoom_out();
    std::cout << std::endl;

    std::cout << "Max zoom :" << std::endl;
    fuffa = test.request_maximum_zoom();
    std::cout << std::endl;

    std::cout << "Absolute zoom 4.5:" << std::endl;
    fuffa = test.set_absolute_zoom(4,5);
    std::cout << std::endl;

    std::cout << "Take a picture:" << std::endl;
    fuffa = test.request_photo();
    std::cout << std::endl;

    std::cout << "Record a video:" << std::endl;
    fuffa = test.request_record();
    std::cout << std::endl;

    std::cout << "Auto focus:" << std::endl;
    fuffa = test.request_autofocus();
    std::cout << std::endl;

    std::cout << "Lock mode:" << std::endl;
    fuffa = test.request_lock_mode();
    std::cout << std::endl;

    std::cout << "Follow mode:" << std::endl;
    fuffa = test.request_follow_mode();
    std::cout << std::endl;

    std::cout << "FPV mode:" << std::endl;
    fuffa = test.request_fpv_mode();
    std::cout << std::endl;

    std::cout << "Send Control Angle (-90,0):" << std::endl;
    fuffa = test.set_gimbal_angles(-90.0f,0.0f);
    std::cout << std::endl;

    std::cout << "////////////////////" << std::endl;
    std::cout << "//   AI_TRACKER   //" << std::endl;
    std::cout << "////////////////////\n" << std::endl;

    std::cout << "heartbeat:" << std::endl;
    tracker->send_heartbeat();
    std::cout << std::endl;

    std::cout << "tracker mode toggle:" << std::endl;
    tracker->request_ai_tracking_switch();
    std::cout << std::endl;

    std::cout << "set tracker mode true:" << std::endl;
    tracker->set_ai_tracking_mode(true);
    std::cout << std::endl;

    std::cout << "set tracker mode false:" << std::endl;
    tracker->set_ai_tracking_mode(false);
    std::cout << std::endl;

    std::cout << "set target:" << std::endl;
    tracker->set_ai_tracking_target(SIYI_AI::AiTracking{.ai_tracking = true, .x_req = 1000, .y_req = 50});
    std::cout << std::endl;
    delete tracker;
    return 0;
}
