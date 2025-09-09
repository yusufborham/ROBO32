/*
 * TFT_int.h
 *
 *  Created on: Sep 2, 2025
 *      Author: drago
 */

#ifndef HAL_TFT_TFT_INT_H_
#define HAL_TFT_TFT_INT_H_


#include "../Fonts/Fonts.h"
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO_int.h"

#define TFT_MADCTL_MY  0x80
#define TFT_MADCTL_MX  0x40
#define TFT_MADCTL_MV  0x20
#define TFT_MADCTL_ML  0x10
#define TFT_MADCTL_RGB 0x00
#define TFT_MADCTL_BGR 0x08
#define TFT_MADCTL_MH  0x04

#define TFT_SPI_PORT MSPI_ID1

#define TFT_RES_Pin       PIN7
#define TFT_RES_GPIO_Port GPIO_PORTA
#define TFT_CS_Pin        PIN6
#define TFT_CS_GPIO_Port  GPIO_PORTB
#define TFT_DC_Pin        PIN9
#define TFT_DC_GPIO_Port  GPIO_PORTA

// AliExpress/eBay 1.8" display, default orientation

/*
#define TFT_IS_160X128 1
#define TFT_WIDTH  128
#define TFT_HEIGHT 160
#define TFT_XSTART 0
#define TFT_YSTART 0
#define TFT_ROTATION (TFT_MADCTL_MX | TFT_MADCTL_MY)
*/

// AliExpress/eBay 1.8" display, rotate right

#define TFT_IS_160X128 1
#define TFT_WIDTH  160
#define TFT_HEIGHT 128
#define TFT_XSTART 0
#define TFT_YSTART 0
#define TFT_ROTATION (TFT_MADCTL_MY | TFT_MADCTL_MV)


// AliExpress/eBay 1.8" display, rotate left
/*
#define TFT_IS_160X128 1
#define TFT_WIDTH  160
#define TFT_HEIGHT 128
#define TFT_XSTART 0
#define TFT_YSTART 0
#define TFT_ROTATION (TFT_MADCTL_MX | TFT_MADCTL_MV)
*/

// AliExpress/eBay 1.8" display, upside down
/*
#define TFT_IS_160X128 1
#define TFT_WIDTH  128
#define TFT_HEIGHT 160
#define TFT_XSTART 0
#define TFT_YSTART 0
#define TFT_ROTATION (0)
*/


#define TFT_NOP     	0x00 //This command is empty command.
#define TFT_SWRESET 	0x01 /*
“-“ Don’t care
-If Software Reset is applied during Sleep In mode, it will be necessary to wait 120msec before
sending next command.
-The display module loads all default values to the registers during 120msec.
-If Software Reset is applied during Sleep Out or Display On Mode, it will be necessary to wait
120msec before sending next command*/

#define TFT_RDDID   	0x04 // -This read byte returns 24-bit display identification information

#define TFT_RDDST   	0x09
#define TFT_RDDPM   	0x0A
#define TFT_RDDMADCTL   0x0B
#define TFT_RDDCOLMOD   0x0C
#define TFT_RDDIM  		0x0D
#define TFT_RDDSM   	0x0E
#define TFT_RDDSDR   	0x0F
#define TFT_SLPIN   	0x10
#define TFT_SLPOUT  	0x11
#define TFT_PTLON   	0x12
#define TFT_NORON   	0x13

#define TFT_INVOFF  	0x20
#define TFT_INVON   	0x21

#define TFT_GAMSET  	0x26

#define TFT_DISPOFF 	0x28
#define TFT_DISPON  	0x29
#define TFT_CASET   	0x2A
#define TFT_RASET   	0x2B
#define TFT_RAMWR   	0x2C

#define TFT_RAMRD   	0x2E

#define TFT_PTLAR   	0x30
#define TFT_SCRLAR   	0x33
#define TFT_TEOFF   	0x34
#define TFT_TEON  	 	0x35
#define TFT_MADCTL   	0x36
#define TFT_VSCSAD   	0x37
#define TFT_IDMOFF   	0x38
#define TFT_IDMON	  	0x39
#define TFT_COLMOD  	0x3A

#define TFT_RDID1   	0xDA
#define TFT_RDID2   	0xDB
#define TFT_RDID3   	0xDC

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

#define TFT_VMOFCTR 	0xC7

#define TFT_WRID2 		0xD1
#define TFT_WRID3 		0xD2

#define TFT_NVCTR1		0xD9

#define TFT_NVCTR2 		0xDE
#define TFT_NVCTR3		0xDF

#define TFT_GMCTRP1 	0xE0
#define TFT_GMCTRN1 	0xE1


#define TFT_COLOR565_RGB(r,g,b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3) )
#define TFT_COLOR565_BGR(r,g,b) ( ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | ((r & 0xF8) >> 3) )


  #define TFT_COLOR565 TFT_COLOR565_RGB   // enable if display needs BGR

// ----------------------------------------------------
// Standard Colors
// ----------------------------------------------------
#define TFT_COLOR_BLACK       TFT_COLOR565(0,   0,   0)
#define TFT_COLOR_WHITE       TFT_COLOR565(255, 255, 255)
#define TFT_COLOR_RED         TFT_COLOR565(255, 0,   0)
#define TFT_COLOR_GREEN       TFT_COLOR565(0,   255, 0)
#define TFT_COLOR_BLUE        TFT_COLOR565(0,   0,   255)
#define TFT_COLOR_YELLOW      TFT_COLOR565(255, 255, 0)
#define TFT_COLOR_CYAN        TFT_COLOR565(0,   255, 255)
#define TFT_COLOR_MAGENTA     TFT_COLOR565(255, 0,   255)

// ----------------------------------------------------
// Extended Colors
// ----------------------------------------------------
#define TFT_COLOR_ORANGE      TFT_COLOR565(255, 165, 0)
#define TFT_COLOR_PURPLE      TFT_COLOR565(128, 0,   128)
#define TFT_COLOR_PINK        TFT_COLOR565(255, 192, 203)
#define TFT_COLOR_BROWN       TFT_COLOR565(165, 42,  42)
#define TFT_COLOR_GRAY        TFT_COLOR565(128, 128, 128)
#define TFT_COLOR_LIGHTGRAY   TFT_COLOR565(211, 211, 211)
#define TFT_COLOR_DARKGRAY    TFT_COLOR565(64,  64,  64)
#define TFT_COLOR_GOLD        TFT_COLOR565(255, 215, 0)
#define TFT_COLOR_SILVER      TFT_COLOR565(192, 192, 192)
#define TFT_COLOR_SKYBLUE     TFT_COLOR565(135, 206, 235)
#define TFT_COLOR_NAVY        TFT_COLOR565(0,   0,   128)
#define TFT_COLOR_MAROON      TFT_COLOR565(128, 0,   0)
#define TFT_COLOR_LIME        TFT_COLOR565(50,  205, 50)
#define TFT_COLOR_TEAL        TFT_COLOR565(0,   128, 128)
#define TFT_COLOR_VIOLET      TFT_COLOR565(238, 130, 238)
#define TFT_COLOR_INDIGO      TFT_COLOR565(75,  0,   130)
#define TFT_COLOR_BEIGE       TFT_COLOR565(245, 245, 220)
#define TFT_COLOR_CORAL       TFT_COLOR565(255, 127, 80)
#define TFT_COLOR_TURQUOISE   TFT_COLOR565(64,  224, 208)

typedef enum {
	GAMMA_10 = 0x01,
	GAMMA_25 = 0x02,
	GAMMA_22 = 0x04,
	GAMMA_18 = 0x08
} GammaDef;


// GPIO init
void HTFT_GPIO_vInit(void);

// Hardware reset
void HTFT_Reset(void);

// TFT init
void HTFT_vInit(void);

// Send command/data
void HTFT_vSendCommand(u8 cmd);
void HTFT_vSendData(u8 data);

// Set drawing window
void HTFT_vSetAddressWindow(u16 x0, u16 y0, u16 x1, u16 y1);
void HTFT_vSetRotation(u8 rotation);

// Draw pixel, color fill
void HTFT_vDrawPixel(u16 x, u16 y, u16 color);
void HTFT_vSendColor(u16 color);
void HTFT_vFillScreen(u16 color);
void HTFT_vFillColor(u16 color);

// Draw shapes
void HTFT_vDrawLine(u16 x0, u16 y0, u16 x1, u16 y1, u16 color);
void HTFT_vDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void HTFT_vFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void HTFT_vDrawTriangle(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void HTFT_vDrawCircle(u16 x0, u16 y0, u16 r, u16 color);

// Draw images
void HTFT_vDrawImage(const u16 *img, u16 width, u16 height);

// Draw text
void HTFT_vDrawChar(u16 x, u16 y, char c, u16 color, u16 bgColor, FontDef font);
void HTFT_vDrawText(u16 x, u16 y, char *text, u16 color, u16 bgColor, FontDef font);

// Display utilities
void Draw_Battery(u8 percent, u16 x, u16 y);
void HTFT_vShowMode(char *modeName);
void HTFT_vShowSensorData(u16 distance, u8 lineDetected);
void HTFT_vShowAlert(char *msg);
void HTFT_vShowStartupScreen(void);

// Test / demo screen
void HTFT_vTestScreen(void);


#endif /* HAL_TFT_TFT_INT_H_ */
