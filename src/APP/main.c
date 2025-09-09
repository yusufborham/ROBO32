#include "Includes_int.h"
#include <stdio.h>
#include <string.h>

// Simple hardware test version
char testBuffer[300];

// Function to display status on TFT
void DisplayStatus(const char* message, u16 color, u8 line) {
    // Clear the line first
    HTFT_vDrawText(10, 10 + (line * 25), "                        ", TFT_COLOR_BLACK, TFT_COLOR_BLACK, Font_16x26);
    HTFT_vDrawText(10, 10 + (line * 25), message, color, TFT_COLOR_BLACK, Font_16x26);
}

// Simple receive function for testing
u8 SimpleReceive(char* buffer, u32 size, u32 timeout_ms) {
    u32 index = 0;
    memset(buffer, 0, size);

    for (u32 i = 0; i < (timeout_ms * 10); i++) {
        u8 c = MUSART_u8ReadByte(USART_PERIPH_1);
        if (c != 0 && c != 0xFF && index < size - 1) {
            buffer[index++] = c;
            buffer[index] = '\0';

            // Check for complete response
            if (strstr(buffer, "\r\n") || strstr(buffer, "OK") || strstr(buffer, "ERROR")) {
                return 1; // Success
            }
        }
        MSYSTICK_vSetDelay_ms(1); // 1ms delay per loop
    }
    return (index > 0) ? 1 : 0;
}

int main(void) {
    // ------------------- RCC -------------------
    MRCC_vInit();
    MRCC_vSetAHBPrescaler(AHB_PRESCALER_DIVIDE_1);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);
    MRCC_vEnableClk(RCC_APB2, RCC_USART1);
    MRCC_vEnableClk(RCC_APB2, RCC_SPI1);

    // ------------------- GPIO -------------------
    // TX Pin (PA1) - Changed to PA9 (standard USART1 TX)
    GPIOx_PinConfig_t txPin = {.port = GPIO_PORTA, .pin = PIN1, .mode = GPIO_MODE_ALTFUNC,
                               .outputType = GPIO_PUSHPULL, .speed = GPIO_HIGH_SPEED,
                               .pull = GPIO_PULLUP, .altFunc = GPIO_AF7_USART1_USART2};
    MGPIO_vPinInit(&txPin);

    // RX Pin (PA2) - Changed to PA10 (standard USART1 RX)
    GPIOx_PinConfig_t rxPin = {.port = GPIO_PORTA, .pin = PIN2, .mode = GPIO_MODE_ALTFUNC,
                               .outputType = GPIO_PUSHPULL, .speed = GPIO_HIGH_SPEED,
                               .pull = GPIO_PULLUP, .altFunc = GPIO_AF7_USART1_USART2};
    MGPIO_vPinInit(&rxPin);

    // LED Pin
    GPIOx_PinConfig_t ledPin = {.port = GPIO_PORTA, .pin = PIN3, .mode = GPIO_MODE_OUTPUT,
                                .outputType = GPIO_PUSHPULL, .speed = GPIO_HIGH_SPEED, .pull = GPIO_PULLDOWN};
    MGPIO_vPinInit(&ledPin);

    // ------------------- USART -------------------
    USART_Config_t usart1_cfg = {
        .peripheral = USART_PERIPH_1,
        .baudRate = USART_BAUDRATE_115200,  // Try 9600 if 115200 doesn't work
        .wordLength = USART_WORD_LENGTH_8BITS,
        .stopBits = USART_STOP_BITS_1,
        .parity = USART_PARITY_NONE,
        .sampleRate = USART_SAMPLE_16_TIMES,
        .sampleMethod = USART_SAMPLE_METHOD_THREE_BITS,
        .mode = USART_MODE_TX_RX,
        .fclk = USART_CLK_25MHZ
    };
    MUSART_Init(&usart1_cfg);

    // ------------------- SysTick -------------------
    MSYSTICK_Config_t stk = { .InterruptEnable = INT_ENABLE, .CLK_SRC = CLK_SRC_AHB_8 };
    MSYSTICK_vInit(&stk);

    // ------------------- TFT -------------------
    HTFT_GPIO_vInit();
    MSPI_vInit(SPI1);
    HTFT_vInit();
    HTFT_vSetRotation(1);
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    DisplayStatus("ESP-01 Hardware Test", TFT_COLOR_WHITE, 0);
    DisplayStatus("====================", TFT_COLOR_WHITE, 1);

    MSYSTICK_vSetDelay_ms(2000);

    // ------------------- Hardware Test Sequence -------------------

    // Test 1: Send AT command and check for any response
    DisplayStatus("Test 1: Basic AT", TFT_COLOR_YELLOW, 3);
    MUSART_u8WriteString(USART_PERIPH_1, "AT\r\n");
    MSYSTICK_vSetDelay_ms(1000);

    if (SimpleReceive(testBuffer, sizeof(testBuffer), 3000)) {
        if (strstr(testBuffer, "OK")) {
            DisplayStatus("AT: OK - ESP responding!", TFT_COLOR_GREEN, 4);
        } else {
            DisplayStatus("AT: Got data but not OK", TFT_COLOR_YELLOW, 4);
            char preview[25];
            snprintf(preview, sizeof(preview), "Data: %.20s", testBuffer);
            DisplayStatus(preview, TFT_COLOR_CYAN, 5);
        }
    } else {
        DisplayStatus("AT: No response!", TFT_COLOR_RED, 4);
        DisplayStatus("Check wiring/power", TFT_COLOR_WHITE, 5);

        // Try different baud rate
        DisplayStatus("Trying 9600 baud...", TFT_COLOR_YELLOW, 6);
        usart1_cfg.baudRate = USART_BAUDRATE_9600;
        MUSART_Init(&usart1_cfg);
        MSYSTICK_vSetDelay_ms(500);

        MUSART_u8WriteString(USART_PERIPH_1, "AT\r\n");
        if (SimpleReceive(testBuffer, sizeof(testBuffer), 3000)) {
            DisplayStatus("9600 baud works!", TFT_COLOR_GREEN, 7);
        } else {
            DisplayStatus("9600 baud failed too", TFT_COLOR_RED, 7);
        }
    }

    MSYSTICK_vSetDelay_ms(3000);

    // Test 2: Get ESP version info
    DisplayStatus("Test 2: Get Version", TFT_COLOR_YELLOW, 8);
    MUSART_u8WriteString(USART_PERIPH_1, "AT+GMR\r\n");
    if (SimpleReceive(testBuffer, sizeof(testBuffer), 3000)) {
        DisplayStatus("Version info received", TFT_COLOR_GREEN, 9);
    } else {
        DisplayStatus("No version info", TFT_COLOR_RED, 9);
    }

    MSYSTICK_vSetDelay_ms(2000);

    // Test 3: Check current WiFi mode
    DisplayStatus("Test 3: Check Mode", TFT_COLOR_YELLOW, 10);
    MUSART_u8WriteString(USART_PERIPH_1, "AT+CWMODE?\r\n");
    if (SimpleReceive(testBuffer, sizeof(testBuffer), 3000)) {
        DisplayStatus("Mode query OK", TFT_COLOR_GREEN, 11);
    } else {
        DisplayStatus("Mode query failed", TFT_COLOR_RED, 11);
    }

    // ------------------- Continuous Test Loop -------------------
    u32 test_counter = 0;

    while (1) {
        test_counter++;

        // Send AT every 5 seconds and show response
        char counter_msg[30];
        snprintf(counter_msg, sizeof(counter_msg), "Test cycle: %lu", test_counter);
        DisplayStatus(counter_msg, TFT_COLOR_WHITE, 13);

        MUSART_u8WriteString(USART_PERIPH_1, "AT\r\n");

        if (SimpleReceive(testBuffer, sizeof(testBuffer), 2000)) {
            DisplayStatus("ESP responding", TFT_COLOR_GREEN, 14);

            // Toggle LED to show activity
            MGPIO_vTogPinValue(GPIO_PORTA, PIN3);

            // Show first 20 chars of response
            char response_preview[25];
            snprintf(response_preview, sizeof(response_preview), "%.20s", testBuffer);
            DisplayStatus(response_preview, TFT_COLOR_CYAN, 15);
        } else {
            DisplayStatus("No ESP response", TFT_COLOR_RED, 14);
            DisplayStatus("Check connections!", TFT_COLOR_WHITE, 15);
        }

        MSYSTICK_vSetDelay_ms(5000);
    }

    return 0;
}

/*
static u8 CurrentPage = 0;
static u8 CurrentMode = 0; // 0 = Normal, 1 = Tank


//---------- Clock Page Helpers ----------
static void Update_Time(void) {
    RTC_DateTime_t now = HRTC_vGetNowWithOffset(2);
    char buffer[20];
    sprintf(buffer, "%02u:%02u:%02u",
            now.time.hours, now.time.minutes, now.time.seconds);

    HTFT_vDrawRectangle(0, 60, TFT_WIDTH, 40, TFT_COLOR_BLACK);

    u16 xPos = (TFT_WIDTH / 2) - (strlen(buffer) * Font_16x26.width / 2);
    HTFT_vDrawText(xPos , 60, buffer, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_16x26);
}

//---------- SysTick Callbacks ----------
static void Clock_Tick_Callback(void) {
    if (CurrentPage == 1) {
        Update_Time();
    }
}

static void Battery_Tick_Callback(void) {
    if (CurrentPage == 2) {
        Battery_Page();   // refresh battery display
    }
}

// ---------- Pages ----------
static void First_Page(void) {
    CurrentPage = 0;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    const char *msg = "ARMatron";
    u16 xPos = (TFT_WIDTH / 2) - (strlen(msg) * Font_16x26.width / 2);
    HTFT_vDrawText(xPos , 50, msg, TFT_COLOR_RED, TFT_COLOR_BLACK, Font_16x26);
}

static void Clock_Page(void) {
    CurrentPage = 1;
    HTFT_vFillScreen(TFT_COLOR_BLACK);
    HTFT_vDrawText(40, 20, "Clock", TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_16x26);

    Update_Time();
}

void Battery_Page(void) {
    CurrentPage = 2;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    HTFT_vDrawText(30, 20, "Battery", TFT_COLOR_CYAN, TFT_COLOR_BLACK, Font_16x26);

    // TODO: replace this with ADC reading
    u8 batteryPercent = 49;

    Draw_Battery(batteryPercent, 40, 60);

    char buffer[10];
    sprintf(buffer, "%u%%", batteryPercent);
    HTFT_vDrawText(90, 60, buffer, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_16x26);
}

static void Mode_Page(void) {
    CurrentPage = 3;
    HTFT_vFillScreen(TFT_COLOR_BLACK);

    HTFT_vDrawText(40, 20, "Mode", TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_16x26);

    // Choose which word to display first line
    const char *firstLine  = (CurrentMode == 0) ? "Normal" : "Tank";
    const char *secondLine = "Steering";

    // First line (Normal or Tank)
    u16 x1 = (TFT_WIDTH / 2) - (strlen(firstLine) * Font_16x26.width / 2);
    HTFT_vDrawText(x1, 60, firstLine, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_16x26);

    // Second line (Steering)
    u16 x2 = (TFT_WIDTH / 2) - (strlen(secondLine) * Font_16x26.width / 2);
    HTFT_vDrawText(x2, 100, secondLine, TFT_COLOR_GREEN, TFT_COLOR_BLACK, Font_16x26);
}


static void Toggle_Mode(void) {
    CurrentMode = !CurrentMode; // switch between 0 and 1
    if (CurrentPage == 3) {
        Mode_Page();  // refresh display if we are on Mode page
    }
}



static void onIrCode(u8 code) {
    switch(code) {
        case IR_CODE_POWER:
            First_Page();
            break;

        case IR_CODE_0:
            Clock_Page();
            break;

        case IR_CODE_1:
            Battery_Page();
            break;

        case IR_CODE_2:   // Show Mode page
            Mode_Page();
            break;

        case IR_CODE_MODE:   // Change mode
            Toggle_Mode();
            break;

        default:
            break;
    }
}


// ---------- Main ----------
int main(void) {
    MRCC_vInit();
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);
    MRCC_vEnableClk(RCC_APB2, RCC_SPI1);
    MRCC_vEnableClk(RCC_APB2, RCC_USART1);

    MSYSTICK_Config_t stk = {
        .InterruptEnable = INT_ENABLE,
        .CLK_SRC = CLK_SRC_AHB_8
    };
    MSYSTICK_vInit(&stk);

    // 1s interval for updating time and battery
    MSYSTICK_vSetInterval_Multi(1000, Clock_Tick_Callback);
    MSYSTICK_vSetInterval_Multi(2000, Battery_Tick_Callback); // refresh battery every 2s

    HTFT_GPIO_vInit();
    MSPI_vInit(SPI1);
    HRTC_vInit();

    HTFT_vInit();
    HTFT_vSetRotation(1);
    First_Page();

    IR_Init(onIrCode);

    while(1) {
        // idle loop
    }
}
*/
