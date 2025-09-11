/*
 * TFT_int.h
 * Enhanced interface file for TFT driver with UI functions
 *  Created on: Sep 2, 2025
 *      Author: drago
 */

#ifndef HAL_TFT_TFT_INT_H_
#define HAL_TFT_TFT_INT_H_

#include "../Fonts/Fonts.h"
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO_int.h"

// MADCTL register bits
#define TFT_MADCTL_MY  0x80
#define TFT_MADCTL_MX  0x40
#define TFT_MADCTL_MV  0x20
#define TFT_MADCTL_ML  0x10
#define TFT_MADCTL_RGB 0x00
#define TFT_MADCTL_BGR 0x08
#define TFT_MADCTL_MH  0x04

// Hardware pin definitions
#define TFT_SPI_PORT MSPI_ID1
#define TFT_RES_Pin       PIN6
#define TFT_RES_GPIO_Port GPIO_PORTA
#define TFT_CS_Pin        PIN6
#define TFT_CS_GPIO_Port  GPIO_PORTB
#define TFT_DC_Pin        PIN9
#define TFT_DC_GPIO_Port  GPIO_PORTA

// Display dimensions (1.8" 160x128 rotated)
#define TFT_IS_160X128 1
#define TFT_WIDTH  160
#define TFT_HEIGHT 128
#define TFT_XSTART 0
#define TFT_YSTART 0
#define TFT_ROTATION (TFT_MADCTL_MY | TFT_MADCTL_MV)

// ST7735 Commands
#define TFT_NOP     	0x00
#define TFT_SWRESET 	0x01
#define TFT_RDDID   	0x04
#define TFT_RDDST   	0x09
#define TFT_SLPIN   	0x10
#define TFT_SLPOUT  	0x11
#define TFT_PTLON   	0x12
#define TFT_NORON   	0x13
#define TFT_INVOFF  	0x20
#define TFT_INVON   	0x21
#define TFT_DISPOFF 	0x28
#define TFT_DISPON  	0x29
#define TFT_CASET   	0x2A
#define TFT_RASET   	0x2B
#define TFT_RAMWR   	0x2C
#define TFT_RAMRD   	0x2E
#define TFT_MADCTL   	0x36
#define TFT_COLMOD  	0x3A
#define TFT_FRMCTR1 	0xB1
#define TFT_FRMCTR2 	0xB2
#define TFT_FRMCTR3 	0xB3
#define TFT_INVCTR  	0xB4
#define TFT_PWCTR1  	0xC0
#define TFT_PWCTR2  	0xC1
#define TFT_PWCTR3  	0xC2
#define TFT_PWCTR4  	0xC3
#define TFT_PWCTR5  	0xC4
#define TFT_VMCTR1  	0xC5
#define TFT_GMCTRP1 	0xE0
#define TFT_GMCTRN1 	0xE1

// Color macros
#define TFT_COLOR565_RGB(r,g,b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3) )
#define TFT_COLOR565_BGR(r,g,b) ( ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | ((r & 0xF8) >> 3) )
#define TFT_COLOR565 TFT_COLOR565_RGB

// Standard Colors
#define TFT_COLOR_BLACK       0x0000
#define TFT_COLOR_WHITE       0xFFFF
#define TFT_COLOR_RED         0xF800
#define TFT_COLOR_GREEN       0x07E0
#define TFT_COLOR_BLUE        0x001F
#define TFT_COLOR_YELLOW      0xFFE0
#define TFT_COLOR_CYAN        0x07FF
#define TFT_COLOR_MAGENTA     0xF81F
#define TFT_COLOR_ORANGE      0xFC00
#define TFT_COLOR_PURPLE      0x8010
#define TFT_COLOR_PINK        0xFC18
#define TFT_COLOR_BROWN       0xA145
#define TFT_COLOR_GRAY        0x8410
#define TFT_COLOR_LIGHTGRAY   0xC618
#define TFT_COLOR_DARKGRAY    0x4208

// Error codes
typedef enum {
    TFT_OK = 0,
    TFT_ERROR_TIMEOUT = 1,
    TFT_ERROR_INVALID_PARAM = 2,
    TFT_ERROR_INIT_FAILED = 3
} TFT_Result_t;

// Page definitions
typedef enum {
    TFT_PAGE_FIRST = 0,
    TFT_PAGE_CLOCK = 1,
    TFT_PAGE_BATTERY = 2,
    TFT_PAGE_MODE = 3,
    TFT_PAGE_ULTRASONIC = 4,
    TFT_PAGE_ESP = 5,
    TFT_PAGE_IR_SENSORS = 6,
    TFT_PAGE_SENSORS_ALL = 7
} TFT_Page_t;

// Mode definitions
typedef enum {
    TFT_MODE_NORMAL = 0,
    TFT_MODE_TANK = 1
} TFT_Mode_t;

// ============================================================================
// BASIC TFT DRIVER FUNCTIONS
// ============================================================================

// Low-level functions
void HTFT_GPIO_vInit(void);
void HTFT_vReset(void);
void HTFT_vInit(void);
void HTFT_vSendCommand(u8 cmd);
void HTFT_vSendData(u8 data);
void HTFT_vSetAddressWindow(u16 x0, u16 y0, u16 x1, u16 y1);
void HTFT_vSetRotation(u8 rotation);

// Drawing functions
void HTFT_vDrawPixel(u16 x, u16 y, u16 color);
void HTFT_vSendColor(u16 color);
void HTFT_vFillScreen(u16 color);
void HTFT_vDrawLine(u16 x0, u16 y0, u16 x1, u16 y1, u16 color);
void HTFT_vDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void HTFT_vFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void HTFT_vDrawCircle(u16 x0, u16 y0, u16 r, u16 color);
void HTFT_vDrawImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize);
void HTFT_vDrawChar(u16 x, u16 y, char c, u16 color, u16 bgColor, FontDef font);
void HTFT_vDrawText(u16 x, u16 y, char *text, u16 color, u16 bgColor, FontDef font);

// Battery drawing function
void Draw_Battery(u8 percent, u16 x, u16 y);

// ============================================================================
// UI MANAGEMENT FUNCTIONS (NEW)
// ============================================================================

// State management functions
void HTFT_vSetCurrentDistance(float distance);
float HTFT_vGetCurrentDistance(void);
void HTFT_vSetESPStatus(u8 status);
u8 HTFT_vGetESPStatus(void);
u8 HTFT_vGetCurrentPage(void);
u8 HTFT_vGetCurrentMode(void);

// Page display functions
void HTFT_vShowFirstPage(void);
void HTFT_vShowClockPage(void);
void HTFT_vShowBatteryPage(void);
void HTFT_vShowModePage(void);
void HTFT_vShowUltrasonicPage(void);
void HTFT_vShowESPPage(void);
void HTFT_vShowIRSensorsPage(void);
void HTFT_vShowSensorsPage(float distance, int batteryPercent);

// Control functions
void HTFT_vToggleMode(void);
void HTFT_vNavigateToPage(u8 pageNumber);

// Update functions
void HTFT_vUpdateTime(void);
void HTFT_vUpdateBatteryDisplay(void);
void HTFT_vUpdateUltrasonicDisplay(void);
void HTFT_vUpdateESPStatus(void);

// ============================================================================
// LEGACY APPLICATION FUNCTIONS (COMPATIBILITY)
// ============================================================================

void HTFT_vShowMode(char *modeName);
void HTFT_vShowSensorData(u16 distance, u8 lineDetected);
void HTFT_vShowAlert(char *msg);

// ============================================================================
// CONFIGURATION UTILITY FUNCTIONS
// ============================================================================

u32 HTFT_GetTimeout(void);
u16 HTFT_GetChunkSize(void);
u8 HTFT_IsByteSwapEnabled(void);
u8 HTFT_IsHalSpiHandleUsed(void);

#endif /* HAL_TFT_TFT_INT_H_ */
