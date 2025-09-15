/*
 * HTFT_prg.c
 * Enhanced STM32 SPI TFT Driver with Configuration Support and UI Functions
 * Pins: SCK=PA5, MOSI=PA7, CS=PB6, DC=PA9, RESET=PA6
 */

#include "TFT_prv.h"
#include "TFT_int.h"
#include "TFT_cfg.h"
#include "../../MCAL/SPI/SPI_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/RTC/RTC_int.h"
#include "../Fonts/Fonts.h"
#include <string.h>
#include <stdio.h>
#include "../../Robo_int/Battery_level.h"

#define TFT_CHUNK TFT_TX_CHUNK_PIXELS

// ---------------- UI State Variables ----------------
static u8 CurrentPage = 0;       // 0=First, 1=Clock, 2=Battery, 3=Mode, 4=Ultrasonic, 5=ESP
static u8 CurrentMode = 0;       // 0=Normal, 1=Tank
static float current_distance = 0.0f;
static u8 esp_status = 0;        // 0=Disconnected, 1=Connected

// ---------------- Configuration-based Helper Functions ----------------

/**
 * @brief Swap bytes in 16-bit color value if configured
 * @param color: Original 16-bit color value
 * @return Swapped color value if TFT_SWAP_BYTES_IN_TX is enabled
 */
static u16 HTFT_SwapBytes(u16 color) {
#if TFT_SWAP_BYTES_IN_TX
    return ((color & 0xFF) << 8) | ((color >> 8) & 0xFF);
#else
    return color;
#endif
}

/**
 * @brief Send data with timeout and error handling
 * @param data: Pointer to data buffer
 * @param size: Size of data to send
 * @return 1 for success, 0 for timeout/error
 */
static u8 HTFT_SendDataWithTimeout(u8* data, u16 size) {
#if TFT_USE_HAL_SPI_HANDLE
    // Using HAL-style timeout handling
    u32 timeout_start = MSYSTICK_u32GetElapsedTime_SingleShot();
    u32 timeout_ticks = TFT_SPI_TIMEOUT_MS; // Convert to system ticks if needed

    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_LOW);  // CS = 0

    // Send data with timeout monitoring
    for (u16 i = 0; i < size; i++) {
        // Check timeout
        if ((MSYSTICK_u32GetElapsedTime_SingleShot() - timeout_start) > timeout_ticks) {
            MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_HIGH); // CS = 1
            return 0; // Timeout error
        }

        // Send single byte (you might need to adapt this based on your SPI driver)
        MSPI_vTransmitBuffer(SPI1, &data[i], 1);
    }

    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_HIGH); // CS = 1
    return 1; // Success
#else
    // Simple send without timeout
    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_LOW);  // CS = 0
    MSPI_vTransmitBuffer(SPI1, data, size);
    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_HIGH); // CS = 1
    return 1;
#endif
}

// ---------------- GPIO Initialization ----------------
void HTFT_GPIO_vInit(void) {
    GPIOx_PinConfig_t cfg;

    // RESET (PA6)
    cfg.port = GPIO_PORTB; cfg.pin = PIN14;
    cfg.mode = GPIO_MODE_OUTPUT; cfg.outputType = GPIO_PUSHPULL;
    cfg.speed = GPIO_HIGH_SPEED; cfg.pull = GPIO_NOPULL;
    MGPIO_vPinInit(&cfg);

    // DC (PA9)
    cfg.pin = PIN15; MGPIO_vPinInit(&cfg);

    // CS (PB6)
    cfg.port = GPIO_PORTB; cfg.pin = PIN13; MGPIO_vPinInit(&cfg);

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
    MGPIO_vSetPinValue(GPIO_PORTB, PIN14, GPIO_LOW);
    MSYSTICK_vSetDelay_ms(100);
    MGPIO_vSetPinValue(GPIO_PORTB, PIN14, GPIO_HIGH);
    MSYSTICK_vSetDelay_ms(200);
}

// ---------------- Low-level Command/Data with Configuration Support ----------------
void HTFT_vSendCommand(u8 cmd) {
    MGPIO_vSetPinValue(GPIO_PORTB, PIN15, GPIO_LOW);  // DC = 0
    HTFT_SendDataWithTimeout(&cmd, 1);
}

void HTFT_vSendData(u8 data) {
    MGPIO_vSetPinValue(GPIO_PORTB, PIN15, GPIO_HIGH); // DC = 1
    HTFT_SendDataWithTimeout(&data, 1);
}

// ---------------- Enhanced Color Sending with Configuration Support ----------------
void HTFT_vSendColor(u16 color) {
    u16 swapped_color = HTFT_SwapBytes(color);
    u8 high = swapped_color >> 8;
    u8 low = swapped_color & 0xFF;

    MGPIO_vSetPinValue(GPIO_PORTB, PIN15, GPIO_HIGH); // DC = 1
    u8 color_data[2] = {high, low};
    HTFT_SendDataWithTimeout(color_data, 2);
}

// ---------------- Optimized Bulk Color Transfer ----------------
static void HTFT_vSendColorBulk(u16* colors, u32 count) {
    if (count == 0) return;

    MGPIO_vSetPinValue(GPIO_PORTB, PIN15, GPIO_HIGH); // DC = 1
    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_LOW);  // CS = 0

    // Process colors in chunks for better performance
    u32 remaining = count;
    u32 index = 0;

    while (remaining > 0) {
        u32 chunk_size = (remaining > TFT_CHUNK) ? TFT_CHUNK : remaining;

        for (u32 i = 0; i < chunk_size; i++) {
            u16 swapped_color = HTFT_SwapBytes(colors[index + i]);
            u8 color_bytes[2] = {swapped_color >> 8, swapped_color & 0xFF};
            MSPI_vTransmitBuffer(SPI1, color_bytes, 2);
        }

        remaining -= chunk_size;
        index += chunk_size;

        // Small delay between chunks to prevent overwhelming the display
        if (remaining > 0) {
            MSYSTICK_vSetDelay_ms(1);
        }
    }

    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_HIGH); // CS = 1
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

// ---------------- Optimized Fill Screen ----------------
void HTFT_vFillScreen(u16 color) {
    HTFT_vSetAddressWindow(0, 0, TFT_WIDTH-1, TFT_HEIGHT-1);

    u16 buf[TFT_CHUNK];
    u16 swapped_color = HTFT_SwapBytes(color);

    // Pre-fill buffer with the color
    for(u16 i = 0; i < TFT_CHUNK; i++) {
        buf[i] = swapped_color;
    }

    u32 total = (u32)TFT_WIDTH * (u32)TFT_HEIGHT;

    MGPIO_vSetPinValue(GPIO_PORTB, PIN15, GPIO_HIGH); // DC = 1
    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_LOW);  // CS = 0

    while(total > 0) {
        u32 chunk = (total > TFT_CHUNK) ? TFT_CHUNK : total;

        // Send chunk as raw bytes (2 bytes per pixel)
        MSPI_vTransmitBuffer(SPI1, (u8*)buf, chunk * 2);

        total -= chunk;
    }

    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_HIGH); // CS = 1
}

// ---------------- Enhanced Fill Rectangle ----------------
void HTFT_vFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color) {
    if (x1 > x2) { u16 temp = x1; x1 = x2; x2 = temp; }
    if (y1 > y2) { u16 temp = y1; y1 = y2; y2 = temp; }

    HTFT_vSetAddressWindow(x1, y1, x2, y2);

    u32 width = (u32)x2 - (u32)x1 + 1;
    u32 height = (u32)y2 - (u32)y1 + 1;
    u32 total_pixels = width * height;

    u16 buf[TFT_CHUNK];
    u16 swapped_color = HTFT_SwapBytes(color);

    // Pre-fill buffer
    for(u16 i = 0; i < TFT_CHUNK; i++) {
        buf[i] = swapped_color;
    }

    MGPIO_vSetPinValue(GPIO_PORTB, PIN15, GPIO_HIGH); // DC = 1
    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_LOW);  // CS = 0

    while(total_pixels > 0) {
        u32 chunk = (total_pixels > TFT_CHUNK) ? TFT_CHUNK : total_pixels;
        MSPI_vTransmitBuffer(SPI1, (u8*)buf, chunk * 2);
        total_pixels -= chunk;
    }

    MGPIO_vSetPinValue(GPIO_PORTB, PIN13, GPIO_HIGH); // CS = 1
}

// ---------------- Configuration-aware Initialization ----------------
void HTFT_vSetRotation(u8 rotation) {
    HTFT_vSendCommand(TFT_MADCTL);

    switch(rotation) {
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

void HTFT_vInit(void) {
    HTFT_GPIO_vInit();
    HTFT_vReset();

    HTFT_vSendCommand(TFT_SWRESET);
    MSYSTICK_vSetDelay_ms(150);

    HTFT_vSendCommand(TFT_SLPOUT);
    MSYSTICK_vSetDelay_ms(150);

    HTFT_vSendCommand(TFT_COLMOD);
    HTFT_vSendData(0x55); // 16-bit color

    HTFT_vSendCommand(TFT_MADCTL);
    HTFT_vSendData(TFT_MADCTL_MX | TFT_MADCTL_MY | TFT_MADCTL_BGR);

    HTFT_vSendCommand(TFT_DISPON);
    MSYSTICK_vSetDelay_ms(50);

    HTFT_vFillScreen(TFT_COLOR_BLACK);
}

// ---------------- Draw Pixel with Configuration Support ----------------
void HTFT_vDrawPixel(u16 x, u16 y, u16 color) {
    if (x >= TFT_WIDTH || y >= TFT_HEIGHT) return; // Bounds checking

    HTFT_vSetAddressWindow(x, y, x, y);
    HTFT_vSendColor(color);
}

// ---------------- Basic Drawing Functions ----------------
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

// ---------------- Enhanced Image Drawing with Configuration Support ----------------
void HTFT_vDrawImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize) {
	u8 MSB, LSB;

    // 1.0 Set X position (Column address set)
    HTFT_vSendCommand(TFT_CASET);
    HTFT_vSendData(0x00);
    HTFT_vSendData(0x00);               // X start
    HTFT_vSendData((TFT_WIDTH - 1) >> 8);
    HTFT_vSendData((TFT_WIDTH - 1) & 0xFF);   // X end

    // 2.0 Set Y position (Row address set)
    HTFT_vSendCommand(TFT_RASET);
    HTFT_vSendData(0x00);
    HTFT_vSendData(0x00);               // Y start
    HTFT_vSendData((TFT_HEIGHT - 1) >> 8);
    HTFT_vSendData((TFT_HEIGHT - 1) & 0xFF);  // Y end

    // 3.0 Write to RAM
    HTFT_vSendCommand(TFT_RAMWR);

    // 3.1 Send image pixel data
    for (u16 i = 0; i < A_u16ImgSize; i++) {
        MSB = (A_u16ImgArr[i] >> 8) & 0xFF;
        LSB = A_u16ImgArr[i] & 0xFF;
        HTFT_vSendData(MSB);
        HTFT_vSendData(LSB);
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
        fillColor = TFT_COLOR_RED;
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

// ============================================================================
// UI PAGE FUNCTIONS (moved from main.c)
// ============================================================================

// ---------------- Private UI Helper Functions ----------------
static void DisplayStatus(const char* message, u16 color, u8 line) {
    if (CurrentPage != 5) return; // Only display if on ESP page

    // Clear the line first
    HTFT_vFillRectangle(10, 40 + (line * 15), 200, 55, TFT_COLOR_BLACK);
    HTFT_vDrawText(10, 40 + (line * 15), (char*)message, color, TFT_COLOR_BLACK, Font_8x13);
}

// ---------------- State Management Functions ----------------
void HTFT_vSetCurrentDistance(float distance) {
    current_distance = distance;
}

float HTFT_vGetCurrentDistance(void) {
    return current_distance;
}

void HTFT_vSetESPStatus(u8 status) {
    esp_status = status;
}

u8 HTFT_vGetESPStatus(void) {
    return esp_status;
}

u8 HTFT_vGetCurrentPage(void) {
    return CurrentPage;
}

u8 HTFT_vGetCurrentMode(void) {
    return CurrentMode;
}

// ---------------- Page Display Functions ----------------
void HTFT_vShowFirstPage(void) {
    CurrentPage = 0;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    const char *msg = "ARMatron";
    u16 xPos = (TFT_WIDTH / 2) - (strlen(msg) * Font_8x13.width / 2);
    HTFT_vDrawText(xPos, 50, (char*)msg, TFT_COLOR_RED, TFT_COLOR_BLACK, Font_8x13);
}

void HTFT_vShowClockPage(void) {
    CurrentPage = 1;
    HTFT_vFillScreen(TFT_COLOR_BLACK);
    HTFT_vDrawText(40, 20, "Clock", TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_8x13);

    HTFT_vUpdateTime();
}

void HTFT_vShowBatteryPage(void) {
	Battery_Level_Init();
    CurrentPage = 2;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    HTFT_vDrawText(30, 20, "Battery", TFT_COLOR_CYAN, TFT_COLOR_BLACK, Font_8x13);

    // TODO: replace with actual ADC reading
    //u8 batteryPercent = 85; // Placeholder value

    Draw_Battery(Battery_Level_Read(), 40, 60);

    char buffer[20];
    sprintf(buffer, "%u%%", Battery_Level_Read());
    HTFT_vDrawText(90, 60, buffer, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_8x13);
}

void HTFT_vShowModePage(void) {
    CurrentPage = 3;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    HTFT_vDrawText(40, 20, "Mode", TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_8x13);

    const char *firstLine = (CurrentMode == 0) ? "Normal" : "Tank";
    const char *secondLine = "Steering";

    u16 x1 = (TFT_WIDTH / 2) - (strlen(firstLine) * Font_8x13.width / 2);
    HTFT_vDrawText(x1, 60, (char*)firstLine, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_8x13);

    u16 x2 = (TFT_WIDTH / 2) - (strlen(secondLine) * Font_8x13.width / 2);
    HTFT_vDrawText(x2, 100, (char*)secondLine, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_8x13);
}

void HTFT_vShowUltrasonicPage(void) {
    CurrentPage = 4;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    HTFT_vDrawText(30, 20, "Ultrasonic", TFT_COLOR_CYAN, TFT_COLOR_BLACK, Font_8x13);

    char buffer[30];
    if (current_distance >= 0) {
        sprintf(buffer, "Distance: %.1f cm", current_distance);
        u16 color = (current_distance < 10.0f) ? TFT_COLOR_RED : TFT_COLOR_GREEN;
        HTFT_vDrawText(10, 60, buffer, color, TFT_COLOR_BLACK, Font_8x13);
    } else if (current_distance == -1.0f) {
        HTFT_vDrawText(10, 60, "No object detected", TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_8x13);
    } else {
        HTFT_vDrawText(10, 60, "Sensor Error", TFT_COLOR_RED, TFT_COLOR_BLACK, Font_8x13);
    }

    // Status indicator
    if (current_distance < 10.0f && current_distance >= 0) {
        HTFT_vDrawText(10, 90, "WARNING: Object close!", TFT_COLOR_RED, TFT_COLOR_BLACK, Font_8x13);
    }
}

void HTFT_vShowESPPage(void) {
    CurrentPage = 5;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    HTFT_vDrawText(30, 20, "ESP-01 Status", TFT_COLOR_CYAN, TFT_COLOR_BLACK, Font_8x13);

    if (esp_status) {
        DisplayStatus("ESP: Connected", TFT_COLOR_GREEN, 1);
    } else {
        DisplayStatus("ESP: Disconnected", TFT_COLOR_RED, 1);
    }
}

void HTFT_vShowIRSensorsPage(void) {
    CurrentPage = 6;
    HTFT_vFillScreen(TFT_COLOR_BLACK);
    HTFT_vDrawText(30, 10, "Line Sensors", TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_8x13);

    // Placeholder values - replace with actual sensor readings
    u8 ir1 = 0, ir2 = 0, ir3 = 0, ir4 = 0, ir5 = 0;

    char buffer[40];
    sprintf(buffer, "IR: %d %d %d %d %d", ir1, ir2, ir3, ir4, ir5);
    HTFT_vDrawText(20, 60, buffer, TFT_COLOR_WHITE, TFT_COLOR_BLACK, Font_8x13);
}

void HTFT_vShowSensorsPage(float distance, int batteryPercent) {
    CurrentPage = 7;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    // Battery top-left
    char batt[20];
    sprintf(batt, "Bat: %d%%", batteryPercent);
    HTFT_vDrawText(5, 5, batt, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_8x13);

    // Ultrasonic
    char dist[30];
    sprintf(dist, "US: %.1f cm", distance);
    HTFT_vDrawText(5, 30, dist, TFT_COLOR_WHITE, TFT_COLOR_BLACK, Font_8x13);

    // IR sensors placeholder
    char irbuf[40];
    sprintf(irbuf, "IR: 0 0 0 0 %d", (int)distance);
    HTFT_vDrawText(5, 60, irbuf, TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_8x13);
}

// ---------------- Control Functions ----------------
void HTFT_vToggleMode(void) {
    CurrentMode = !CurrentMode;
    if (CurrentPage == 3) {
        HTFT_vShowModePage();
    }
}

// ---------------- Update Functions ----------------
void HTFT_vUpdateTime(void) {
    if (CurrentPage != 1) return;

    RTC_DateTime_t now = HRTC_vGetNowWithOffset(2);
    char buffer[20];
    sprintf(buffer, "%02u:%02u:%02u",
            now.time.hours, now.time.minutes, now.time.seconds);

    HTFT_vFillRectangle(0, 60, TFT_WIDTH, 100, TFT_COLOR_BLACK);

    u16 xPos = (TFT_WIDTH / 2) - (strlen(buffer) * Font_8x13.width / 2);
    HTFT_vDrawText(xPos, 60, buffer, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_8x13);
}

void HTFT_vUpdateBatteryDisplay(void) {
    if (CurrentPage == 2) {
        HTFT_vShowBatteryPage();
    }
}

void HTFT_vUpdateUltrasonicDisplay(void) {
    if (CurrentPage == 4) {
        HTFT_vShowUltrasonicPage();
    }
}

void HTFT_vUpdateESPStatus(void) {
    if (CurrentPage == 5) {
        // ESP status test would go here
        // For now, just refresh the page
        HTFT_vShowESPPage();
    }
}

// ---------------- Navigation Function ----------------
void HTFT_vNavigateToPage(u8 pageNumber) {
    switch(pageNumber) {
        case 0: HTFT_vShowFirstPage(); break;
        case 1: HTFT_vShowClockPage(); break;
        case 2: HTFT_vShowBatteryPage(); break;
        case 3: HTFT_vShowModePage(); break;
        case 4: HTFT_vShowUltrasonicPage(); break;
        case 5: HTFT_vShowESPPage(); break;
        case 6: HTFT_vShowIRSensorsPage(); break;
        case 7: HTFT_vShowSensorsPage(current_distance, 70); break;
        default: HTFT_vShowFirstPage(); break;
    }
}

// ---------------- Configuration Utility Functions ----------------

/**
 * @brief Get current timeout setting
 * @return Timeout value in milliseconds
 */
u32 HTFT_GetTimeout(void) {
    return TFT_SPI_TIMEOUT_MS;
}

/**
 * @brief Get current chunk size setting
 * @return Chunk size in pixels
 */
u16 HTFT_GetChunkSize(void) {
    return TFT_TX_CHUNK_PIXELS;
}

/**
 * @brief Check if byte swapping is enabled
 * @return 1 if enabled, 0 if disabled
 */
u8 HTFT_IsByteSwapEnabled(void) {
    return TFT_SWAP_BYTES_IN_TX;
}

/**
 * @brief Check if HAL SPI handle is used
 * @return 1 if enabled, 0 if disabled
 */
u8 HTFT_IsHalSpiHandleUsed(void) {
    return TFT_USE_HAL_SPI_HANDLE;
}

// ---------------- Legacy Application Functions ----------------
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
