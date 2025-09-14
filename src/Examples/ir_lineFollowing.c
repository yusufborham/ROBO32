// #include "Includes_int.h"
// #include "../Robo_int/Motor_cmd.h"
// #include "../Robo_int/Servos_cmd.h"
// #include "../HAL/IR_LineFollowing/IR_LineFollowing_int.h"

// int main(){

//     MRCC_vInit();
//     MRCC_vEnableClk(RCC_APB2, RCC_USART1);
//     MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
//     MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);

//     IR_LineFollowing_cfg_t ir_cfg = {
//         .IR_ports = {GPIO_PORTB, GPIO_PORTB, GPIO_PORTB, GPIO_PORTB, GPIO_PORTB},
//         .IR_pins = {PIN0, PIN6, PIN7, PIN8, PIN9},
//         .sensorType = LOW_WHEN_LINE_DETECTED
//     };

//     HIR_vInit(&ir_cfg);

//         // enable tx pin for usart 1
//     GPIOx_PinConfig_t USART1_TX_Pin = {
//        .port = GPIO_PORTA,
//        .pin = PIN2,
//        .mode = GPIO_MODE_ALTFUNC,
//        .speed = GPIO_VHIGH_SPEED,
//        .altFunc = GPIO_AF7_USART1_USART2  // AF7 for USART1
//     };
//     MGPIO_vPinInit(&USART1_TX_Pin);

//     USART_Config_t myUsart = {
//        .fclk = USART_CLK_25MHZ ,
//        .peripheral = USART_PERIPH_2,
//        .baudRate = USART_BAUDRATE_115200,
//        .wordLength = USART_WORD_LENGTH_8BITS,
//        .stopBits = USART_STOP_BITS_1,
//        .parity = USART_PARITY_NONE,
//        .sampleRate = USART_SAMPLE_16_TIMES,
//        .sampleMethod = USART_SAMPLE_METHOD_THREE_BITS,
//        .mode = USART_MODE_TX_ONLY
//     };

//     MUSART_Init(&myUsart);


//     Motor_Init();

//     u8 buffer[20];

//     while (1){
//         f32 position;
//     if (HIR_f32ReadSensors(&ir_cfg, &position) == IR_STATUS_LINE_AQUIRED) { // -2 --> 0 --> +2
//         // Use the position value for control
//         ftoa(position, buffer);
//         MUSART_u8WriteString(USART_PERIPH_2, buffer);
//         const u8 newline[2] = "\n";
//         MUSART_u8WriteString(USART_PERIPH_2, newline);
//         }
//     }
// }