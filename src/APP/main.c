// #include "Includes_int.h"
// #include "../Robo_int/Motor_cmd.h"
// #include "../Robo_int/Servos_cmd.h"


// int main(void) {
//     MRCC_vInit();
//     Motor_Init();
//     Servo_Init();

//     while (1) {
//         // Forward ramp with servo sweep
//         for (u8 duty = 0; duty <= 100; duty += 5) {
//             Motor_FWD(duty);
//             Servo_Steer(duty * 180 / 100);      // Sweep steering servo
//             Servo_Ultasonic(180 - (duty * 180 / 100)); // Sweep ultrasonic servo opposite
//             for (volatile int i = 0; i < 150000; i++);
//         }
//         Motor_Stop();
//         for (volatile int i = 0; i < 500000; i++);

//         // Backward ramp with servo sweep
//         for (u8 duty = 0; duty <= 100; duty += 5) {
//             Motor_BWD(duty);
//             Servo_Steer(180 - (duty * 180 / 100));
//             Servo_Ultasonic(duty * 180 / 100);
//             for (volatile int i = 0; i < 150000; i++);
//         }
//         Motor_Stop();
//         for (volatile int i = 0; i < 500000; i++);

//         // Left motor forward, right motor backward with servo sweep
//         for (u8 duty = 0; duty <= 100; duty += 5) {
//             Motor_Left_FWD(duty);
//             Motor_Right_BWD(duty);
//             Servo_Steer(duty * 180 / 100);
//             Servo_Ultasonic(duty * 180 / 100);
//             for (volatile int i = 0; i < 150000; i++);
//         }
//         Motor_Stop_Left();
//         Motor_Stop_Right();
//         for (volatile int i = 0; i < 500000; i++);
//     }
// }

#include "Includes_int.h"
#include "../Robo_int/Motor_cmd.h"
#include "../Robo_int/Servos_cmd.h"
#include "../HAL/IR_LineFollowing/IR_LineFollowing_int.h"

#define BASE_SPEED 28

u8 Range = 100 - BASE_SPEED ;

// range is from -2 to 2 --> 2 * 15= 30
void move(s8 dir);
void moveLeft(s8 vel);
void moveRight(s8 vel);

void move(s8 dir){

    moveLeft((s8)BASE_SPEED + dir);
    moveRight((s8)BASE_SPEED - dir);
}

void moveLeft(s8 vel){
    vel >= 0 ?  Motor_Left_FWD((u8)abs(vel)) : Motor_Left_BWD((u8)2.5*abs(vel)) ;
}

void moveRight(s8 vel){
    vel >= 0 ?  Motor_Right_FWD((u8)abs(vel)) : Motor_Right_BWD((u8)2.5*abs(vel)) ;
}

int main(){

    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, RCC_USART1);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);

    Motor_Init();
    Servo_Init();

    Servo_Steer(90);

    IR_LineFollowing_cfg_t ir_cfg = {
        .IR_ports = {GPIO_PORTB, GPIO_PORTB, GPIO_PORTB, GPIO_PORTB, GPIO_PORTB},
        .IR_pins = {PIN0, PIN6, PIN7, PIN8, PIN9},
        .sensorType = HIGH_WHEN_LINE_DETECTED
    };

    HIR_vInit(&ir_cfg);

        // enable tx pin for usart 1
    // GPIOx_PinConfig_t USART1_TX_Pin = {
    //    .port = GPIO_PORTA,
    //    .pin = PIN9,
    //    .mode = GPIO_MODE_ALTFUNC,
    //    .speed = GPIO_VHIGH_SPEED,
    //    .altFunc = GPIO_AF7_USART1_USART2  // AF7 for USART1
    // };
    // MGPIO_vPinInit(&USART1_TX_Pin);

    // USART_Config_t myUsart = {
    //    .fclk = USART_CLK_25MHZ ,
    //    .peripheral = USART_PERIPH_1,
    //    .baudRate = USART_BAUDRATE_115200,
    //    .wordLength = USART_WORD_LENGTH_8BITS,
    //    .stopBits = USART_STOP_BITS_1,
    //    .parity = USART_PARITY_NONE,
    //    .sampleRate = USART_SAMPLE_16_TIMES,
    //    .sampleMethod = USART_SAMPLE_METHOD_THREE_BITS,
    //    .mode = USART_MODE_TX_ONLY
    // };

    // MUSART_Init(&myUsart);

    u8 buffer[20];

    // u8 WelcomeMsg[] = "IR Line Following Test\r\n";
    // MUSART_u8WriteString(USART_PERIPH_1, WelcomeMsg);

    u8 senVal[5] = {0};

    while (1){
        f32 position;
        if (HIR_f32ReadSensors(&ir_cfg, &position, senVal) == IR_STATUS_LINE_AQUIRED) { // -2 --> 0 --> +2
            // Use the position value for control
            // for (u8 i = 0; i < 5; i++) {
            //    u8 chr = senVal[i] ? '1' : '0'; // Convert to ASCII '1' or '0'
            //     // send the usart value for each sensor
            //     MUSART_u8WriteByte(USART_PERIPH_1, chr);
            //     MUSART_u8WriteByte(USART_PERIPH_1, ' '); // Space between sensor values
            // }   
            // MUSART_u8WriteByte(USART_PERIPH_1, '\r'); // Carriage return
            // MUSART_u8WriteByte(USART_PERIPH_1, '\n'); // New line
            // ftoa(position, buffer);
            // MUSART_u8WriteString(USART_PERIPH_1, buffer);
            // const u8 newline[2] = "\n";
            // MUSART_u8WriteString(USART_PERIPH_1, newline);
            move(position * 25.0 ); // map -2 --> 2 to -Range --> Range
        }
        else{
            Motor_Stop();
        }
    }

       
}

