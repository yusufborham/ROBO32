#include "Includes_int.h"
#include "../Robo_int/Motor_cmd.h"
#include "../Robo_int/Servos_cmd.h"


int main(void) {
    MRCC_vInit();
    Motor_Init();
    Servo_Init();

    while (1) {
        // Forward ramp with servo sweep
        for (u8 duty = 0; duty <= 100; duty += 5) {
            Motor_FWD(duty);
            Servo_Steer(duty * 180 / 100);      // Sweep steering servo
            Servo_Ultasonic(180 - (duty * 180 / 100)); // Sweep ultrasonic servo opposite
            for (volatile int i = 0; i < 150000; i++);
        }
        Motor_Stop();
        for (volatile int i = 0; i < 500000; i++);

        // Backward ramp with servo sweep
        for (u8 duty = 0; duty <= 100; duty += 5) {
            Motor_BWD(duty);
            Servo_Steer(180 - (duty * 180 / 100));
            Servo_Ultasonic(duty * 180 / 100);
            for (volatile int i = 0; i < 150000; i++);
        }
        Motor_Stop();
        for (volatile int i = 0; i < 500000; i++);

        // Left motor forward, right motor backward with servo sweep
        for (u8 duty = 0; duty <= 100; duty += 5) {
            Motor_Left_FWD(duty);
            Motor_Right_BWD(duty);
            Servo_Steer(duty * 180 / 100);
            Servo_Ultasonic(duty * 180 / 100);
            for (volatile int i = 0; i < 150000; i++);
        }
        Motor_Stop_Left();
        Motor_Stop_Right();
        for (volatile int i = 0; i < 500000; i++);
    }
}
