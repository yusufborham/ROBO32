/*
 * HTFT_prg.c
 * Clean STM32 SPI TFT Driver
 * Pins: SCK=PA5, MOSI=PA7, CS=PB6, DC=PA9, RESET=PA6
 */

#include "TFT_prv.h"
#include "TFT_int.h"
#include "TFT_cfg.h"
#include "../../MCAL/SPI/SPI_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../Fonts/Fonts.h"
#include <string.h>
#include <stdio.h>

#define TFT_CHUNK 64  // Removed the semicolon

// ---------------- GPIO Initialization ----------------
void HTFT_GPIO_vInit(void) {
    GPIOx_PinConfig_t cfg;

    // RESET (PA6)
    cfg.port = GPIO_PORTA; cfg.pin = PIN6;
    cfg.mode = GPIO_MODE_OUTPUT; cfg.outputType = GPIO_PUSHPULL;
    cfg.speed = GPIO_HIGH_SPEED; cfg.pull = GPIO_NOPULL;
    MGPIO_vPinInit(&cfg);

    // DC (PA9)
    cfg.pin = PIN9; MGPIO_vPinInit(&cfg);

    // CS (PB6)
    cfg.port = GPIO_PORTB; cfg.pin = PIN6; MGPIO_vPinInit(&cfg);

    // SCK (PA5) AF5
    cfg.port = GPIO_PORTA; cfg.pin = PIN5; cfg.mode = GPIO_MODE_ALTFUNC;
    MGPIO_vPinInit(&cfg);
    MGPIO_vSetAlt(GPIO_PORTA, PIN5, GPIO_AF5_SPI1_SPI2);

    // MOSI (PA7) AF5
    cfg.pin = PIN7; MGPIO_vPinInit(&cfg);
    MGPIO_vSetAlt(GPIO_PORTA, PIN7, GPIO_AF5_SPI1_SPI2);
}

// ---------------- Hardware Reset ----------------
void HTFT_vReset(void) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN6, GPIO_LOW);
    MSYSTICK_vSetDelay_ms(100);
    MGPIO_vSetPinValue(GPIO_PORTA, PIN6, GPIO_HIGH);
    MSYSTICK_vSetDelay_ms(200);
}

// ---------------- Low-level Command/Data ----------------
void HTFT_vSendCommand(u8 cmd) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN9, GPIO_LOW);  // DC = 0
    MGPIO_vSetPinValue(GPIO_PORTB, PIN6, GPIO_LOW);  // CS = 0
    MSPI_vTransmitBuffer(SPI1, &cmd, 1);
    MGPIO_vSetPinValue(GPIO_PORTB, PIN6, GPIO_HIGH); // CS = 1
}

void HTFT_vSendData(u8 data) {
    MGPIO_vSetPinValue(GPIO_PORTA, PIN9, GPIO_HIGH); // DC = 1
    MGPIO_vSetPinValue(GPIO_PORTB, PIN6, GPIO_LOW);  // CS = 0
    MSPI_vTransmitBuffer(SPI1, &data, 1);
    MGPIO_vSetPinValue(GPIO_PORTB, PIN6, GPIO_HIGH); // CS = 1
}

// ---------------- Address Window ----------------
void HTFT_vSetAddressWindow(u16 x0, u16 y0, u16 x1, u16 y1) {
    HTFT_vSendCommand(TFT_CASET);
    HTFT_vSendData(x0 >> 8); HTFT_vSendData(x0 & 0xFF);
    HTFT_vSendData(x1 >> 8); HTFT_vSendData(x1 & 0xFF);

    HTFT_vSendCommand(TFT_RASET);
    HTFT_vSendData(y0 >> 8); HTFT_vSendData(y0 & 0xFF);
    HTFT_vSendData(y1 >> 8); HTFT_vSendData(y1 & 0xFF);

    HTFT_vSendCommand(TFT_RAMWR);
}

// ---------------- Send Color ----------------
void HTFT_vSendColor(u16 color) {
    u8 high = color >> 8;
    u8 low  = color & 0xFF;
    HTFT_vSendData(high);
    HTFT_vSendData(low);
}

// ---------------- Fill Screen ----------------
void HTFT_vFillScreen(u16 color) {
    HTFT_vSetAddressWindow(0, 0, TFT_WIDTH-1, TFT_HEIGHT-1);
    u16 buf[TFT_CHUNK];
    for(u8 i=0;i<TFT_CHUNK;i++) buf[i] = color;
    u32 total = TFT_WIDTH * TFT_HEIGHT;

    while(total) {
        u32 chunk = (total > TFT_CHUNK) ? TFT_CHUNK : total;
        for(u32 i=0;i<chunk;i++) HTFT_vSendColor(buf[i]);
        total -= chunk;
    }
}

void HTFT_vSetRotation(u8 rotation)
{
    HTFT_vSendCommand(TFT_MADCTL); // MADCTL command

    switch(rotation)
    {
        case 0: // Portrait
            HTFT_vSendData(TFT_MADCTL_MX | TFT_MADCTL_MY | TFT_MADCTL_RGB);
            break;

        case 1: // Landscape
            HTFT_vSendData(TFT_MADCTL_MV | TFT_MADCTL_MY | TFT_MADCTL_RGB);
            break;

        case 2: // Portrait (flipped)
            HTFT_vSendData(TFT_MADCTL_RGB);
            break;

        case 3: // Landscape (flipped)
            HTFT_vSendData(TFT_MADCTL_MV | TFT_MADCTL_MX | TFT_MADCTL_RGB);
            break;
    }
}


// ---------------- Initialization ----------------
void HTFT_vInit(void) {
    HTFT_GPIO_vInit();
    HTFT_vReset();

    HTFT_vSendCommand(TFT_SWRESET);
    MSYSTICK_vSetDelay_ms(150);

    HTFT_vSendCommand(TFT_SLPOUT);
    MSYSTICK_vSetDelay_ms(150);

    HTFT_vSendCommand(TFT_COLMOD);
    HTFT_vSendData(0x55); // 16-bit

    HTFT_vSendCommand(TFT_MADCTL);
    HTFT_vSendData(TFT_MADCTL_MX | TFT_MADCTL_MY | TFT_MADCTL_BGR);

    HTFT_vSendCommand(TFT_DISPON);
    MSYSTICK_vSetDelay_ms(50);

    HTFT_vFillScreen(TFT_COLOR_BLACK);
}

// ---------------- Draw Pixel ----------------
void HTFT_vDrawPixel(u16 x, u16 y, u16 color) {
    HTFT_vSetAddressWindow(x, y, x, y);
    HTFT_vSendColor(color);
}

// ---------------- Draw Helpers ----------------
static void DrawLineCore(u16 x0, u16 y0, u16 x1, u16 y1, u16 color) {
    int dx = (x1>x0)?(x1-x0):(x0-x1);
    int sx = (x0<x1)?1:-1;
    int dy = (y1>y0)?(y0-y1):(y1-y0);
    int sy = (y0<y1)?1:-1;
    int err = dx + dy;

    while(1) {
        HTFT_vDrawPixel(x0, y0, color);
        if(x0==x1 && y0==y1) break;
        int e2=2*err;
        if(e2>=dy){ err+=dy; x0+=sx; }
        if(e2<=dx){ err+=dx; y0+=sy; }
    }
}

void HTFT_vDrawLine(u16 x0,u16 y0,u16 x1,u16 y1,u16 color){ DrawLineCore(x0,y0,x1,y1,color); }
void HTFT_vDrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2,u16 color){
    DrawLineCore(x1,y1,x2,y1,color);
    DrawLineCore(x1,y2,x2,y2,color);
    DrawLineCore(x1,y1,x1,y2,color);
    DrawLineCore(x2,y1,x2,y2,color);
}
void HTFT_vFillRectangle(u16 x1,u16 y1,u16 x2,u16 y2,u16 color){
    HTFT_vSetAddressWindow(x1,y1,x2,y2);
    u32 width=(u32)x2-(u32)x1+1;
    u32 height=(u32)y2-(u32)y1+1;
    for(u32 i=0;i<width*height;i++) HTFT_vSendColor(color);
}
// ---------------- Draw Pixel / Shapes ----------------
void HTFT_vDrawCircle(u16 x0, u16 y0, u16 r, u16 color){
    int x=r, y=0, err=0;
    while(x>=y){
        HTFT_vDrawPixel(x0+x, y0+y,color);
        HTFT_vDrawPixel(x0+y, y0+x,color);
        HTFT_vDrawPixel(x0-y, y0+x,color);
        HTFT_vDrawPixel(x0-x, y0+y,color);
        HTFT_vDrawPixel(x0-x, y0-y,color);
        HTFT_vDrawPixel(x0-y, y0-x,color);
        HTFT_vDrawPixel(x0+y, y0-x,color);
        HTFT_vDrawPixel(x0+x, y0-y,color);
        y++;
        if(err<=0) err+=2*y+1;
        if(err>0){ x--; err-=2*x+1; }
    }
}

// ---------------- Draw Character/Text ----------------
void HTFT_vDrawChar(u16 x, u16 y, char c, u16 color, u16 bgColor, FontDef font){
    if(c<32 || c>126) return;
    u32 idx = (c-32)*font.height;
    for(u8 row=0; row<font.height; row++){
        u16 rowBits = font.data[idx+row];
        for(u8 col=0; col<font.width; col++){
            HTFT_vDrawPixel(x+col, y+row, (rowBits & (1<<(font.width-1-col))) ? color : bgColor);
        }
    }
}

void HTFT_vDrawText(u16 x, u16 y, char *text, u16 color, u16 bgColor, FontDef font){
    while(*text){
        HTFT_vDrawChar(x, y, *text, color, bgColor, font);
        x += font.width;
        text++;
    }
}

// ---------------- Fill Color / Image ----------------
void HTFT_vDrawImage(const u16 *img, u16 width, u16 height){
    HTFT_vSetAddressWindow(0,0,width-1,height-1);
    u32 total = width*height;
    u32 idx=0;
    u16 buf[TFT_CHUNK];
    while(total){
        u32 chunk = (total > TFT_CHUNK)? TFT_CHUNK: total;
        for(u32 i=0;i<chunk;i++) buf[i] = img[idx+i];
        MSPI_vTransmitBuffer(SPI1,(u8*)buf,chunk*2);
        idx += chunk;
        total -= chunk;
    }
}
void Draw_Battery(u8 percent, u16 x, u16 y) {
    u16 width = 40;
    u16 height = 20;

    // Outer battery body
    HTFT_vDrawRectangle(x, y, x + width, y + height, TFT_COLOR_WHITE);

    // Battery head (small rectangle on the right)
    HTFT_vFillRectangle(x + width + 1, y + (height / 4),
                        x + width + 4, y + (3 * height / 4),
                        TFT_COLOR_WHITE);

    // Decide fill color based on percentage
    u16 fillColor;
    if (percent < 20) {
        fillColor = TFT_COLOR_BLUE;
    } else if (percent < 50) {
        fillColor = TFT_COLOR_YELLOW;
    } else {
        fillColor = TFT_COLOR_GREEN;
    }

    // Inner fill width
    u16 fillWidth = (percent * (width - 4)) / 100;

    if (fillWidth > 0) {
        HTFT_vFillRectangle(x + 2, y + 2,
                            x + 2 + fillWidth, y + height - 2,
                            fillColor);
    }
}

void HTFT_vShowMode(char *modeName){
    u16 x=5, y=TFT_HEIGHT-20;
    HTFT_vFillRectangle(x, y, x+100, y+15, TFT_COLOR_BLACK);
    HTFT_vDrawText(x, y,"Mode:", TFT_COLOR_CYAN, TFT_COLOR_BLACK, Font_16x26);
    HTFT_vDrawText(x+40, y, modeName, TFT_COLOR_WHITE, TFT_COLOR_BLACK, Font_16x26);
}

void HTFT_vShowSensorData(u16 distance, u8 lineDetected){
    u16 x=5, y=30;
    char buf[20];
    HTFT_vFillRectangle(x,y,x+120,y+12,TFT_COLOR_BLACK);
    sprintf(buf,"Dist: %d cm", distance);
    HTFT_vDrawText(x,y,buf,TFT_COLOR_YELLOW,TFT_COLOR_BLACK,Font_16x26);
    HTFT_vFillRectangle(x,y+15,x+120,y+27,TFT_COLOR_BLACK);
    HTFT_vDrawText(x,y+15,lineDetected?"Line: YES":"Line: NO",
                  lineDetected?TFT_COLOR_GREEN:TFT_COLOR_RED,TFT_COLOR_BLACK,Font_16x26);
}

void HTFT_vShowAlert(char *msg){
    HTFT_vFillRectangle(0,40,TFT_WIDTH-1,90,TFT_COLOR_BLACK);
    HTFT_vDrawRectangle(10,35,TFT_WIDTH-20,85,TFT_COLOR_RED);
    u16 x=(TFT_WIDTH/2)-(strlen(msg)*Font_16x26.width/2);
    HTFT_vDrawText(x,50,msg,TFT_COLOR_WHITE,TFT_COLOR_BLACK,Font_16x26);
}


