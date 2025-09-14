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
#include "../MCAL/USART/USART_int.h"

int main(void) {
    MRCC_vInit();
    Battery_Level_Init();

    // USART1 TX (PA9) config for printing
    MRCC_vEnableClk(RCC_APB2, RCC_USART1);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);

    GPIOx_PinConfig_t USART1_TX_Pin = {
        .port = GPIO_PORTA,
        .pin = PIN9,
        .mode = GPIO_MODE_ALTFUNC,
        .speed = GPIO_VHIGH_SPEED,
        .outputType = GPIO_PUSHPULL,
        .pull = GPIO_NOPULL,
        .altFunc = GPIO_AF7_USART1_USART2
    };
    MGPIO_vPinInit(&USART1_TX_Pin);

    USART_Config_t myUsart = {
        .fclk = USART_CLK_25MHZ,
        .peripheral = USART_PERIPH_1,
        .baudRate = USART_BAUDRATE_115200,
        .wordLength = USART_WORD_LENGTH_8BITS,
        .stopBits = USART_STOP_BITS_1,
        .parity = USART_PARITY_NONE,
        .sampleRate = USART_SAMPLE_16_TIMES,
        .sampleMethod = USART_SAMPLE_METHOD_THREE_BITS,
        .mode = USART_MODE_TX_ONLY
    };
    MUSART_Init(&myUsart);

    char buffer[16];

    while (1) {
        u8 percent = Battery_Level_Read();

        // Convert percent to string
        int idx = 0;
        u8 temp = percent;
        if (temp == 0) {
            buffer[idx++] = '0';
        } else {
            char rev[4];
            int r = 0;
            while (temp > 0) {
                rev[r++] = (temp % 10) + '0';
                temp /= 10;
            }
            while (r > 0) {
                buffer[idx++] = rev[--r];
            }
        }
        buffer[idx++] = '%';
        buffer[idx++] = '\r';
        buffer[idx++] = '\n';
        buffer[idx] = '\0';

        MUSART_u8WriteString(USART_PERIPH_1, (u8*)buffer);

        for (volatile int i = 0; i < 100000; i++); // Small delay
    }
}

