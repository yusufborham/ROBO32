#include "Seven_Segment_int.h"

static const u8 NumberConfig[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001  // F
};

void _7_Segment_Init(Segment_Init_t* seg) {
    for (int i = 0; i < 8; i++) {
        GPIOx_PinConfig_t pinCfg = {
            .port = seg->Port,
            .pin  = seg->PinNo[i],
            .mode = GPIO_MODE_OUTPUT,
            .outputType = GPIO_PUSHPULL,
            .speed = GPIO_LOW_SPEED
        };
        MGPIO_vPinInit(&pinCfg);
    }
}

void _7_Segment_Write(Segment_Init_t* seg, u8 value) {
    if (value > 0x0F) return;  // invalid input
    u8 pattern = NumberConfig[value];

    for (int i = 0; i < 8; i++) {
        u8 pinState = (pattern >> i) & 0x01;
        MGPIO_vSetPinValue(seg->Port, seg->PinNo[i], pinState);
    }
}

void SEG_DisplayHex(u8 hexValue, Segment_Init_t* seg1, Segment_Init_t* seg2) {
    u8 highNibble = (hexValue >> 4) & 0x0F;
    u8 lowNibble  = hexValue & 0x0F;

    _7_Segment_Write(seg1, highNibble);
    _7_Segment_Write(seg2, lowNibble);
}
