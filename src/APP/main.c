#include "Includes_int.h"

u8 ultrasonic_sensor;

// WiFi connection status
static u8 wifi_connected = 0;
static u8 wifi_init_done = 0;
static u8 connection_attempts = 0;

//Function prototypes
static void SystemInit(void);
static void PeriodicCallbacks_Init(void);
static void onIrCode(u8 code);

// Callback functions for periodic tasks
static void Clock_Tick_Callback(void);
static void Battery_Tick_Callback(void);
static void Ultrasonic_Display_Callback(void);
static void Ultrasonic_Measurement_Callback(void);



//========== SYSTEM INITIALIZATION ==========

static void SystemInit(void) {
    //---------- RCC Configuration ----------
    MRCC_vInit();
    MRCC_vSetAHBPrescaler(AHB_PRESCALER_DIVIDE_1);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);
    MRCC_vEnableClk(RCC_APB2, RCC_SPI1);
    MRCC_vEnableClk(RCC_APB1, RCC_USART2);  // Enable USART2 clock on APB1
    Motor_Init();



    //---------- Ultrasonic Sensor ----------
    //HCSR04_vInit(&ultrasonic_sensor);

    //---------- Warning LED (PA4) - Moved from PA3 to avoid conflict ----------
    GPIOx_PinConfig_t led_cfg = {
        .port = GPIO_PORTA, .pin = PIN15, .mode = GPIO_MODE_OUTPUT,
        .outputType = GPIO_PUSHPULL, .speed = GPIO_LOW_SPEED,
        .pull = GPIO_NOPULL, .altFunc = 0
    };
    MGPIO_vPinInit(&led_cfg);

    //---------- SysTick ----------
    MSYSTICK_Config_t stk = {
        .InterruptEnable = INT_ENABLE,
        .CLK_SRC = CLK_SRC_AHB_8
    };
    MSYSTICK_vInit(&stk);

    //---------- TFT Display ----------
    HTFT_GPIO_vInit();
    MSPI_vInit(SPI1);
    HTFT_vInit();
    HTFT_vSetRotation(1);

    //---------- RTC ----------
    HRTC_vInit();

    //---------- WiFi Initialization Variables ----------
    wifi_connected = 0;
    wifi_init_done = 0;
    connection_attempts = 0;
    HTFT_vSetESPStatus(0);  // Initially disconnected
}

//========== PERIODIC CALLBACK SETUP ==========

static void PeriodicCallbacks_Init(void) {
    // Set up periodic callbacks using SysTick
    MSYSTICK_vSetInterval_Multi(1000, Clock_Tick_Callback);         // 1s for clock updates
    MSYSTICK_vSetInterval_Multi(2000, Battery_Tick_Callback);       // 2s for battery updates
    MSYSTICK_vSetInterval_Multi(500, Ultrasonic_Display_Callback);  // 0.5s for ultrasonic display updates
    MSYSTICK_vSetInterval_Multi(300, Ultrasonic_Measurement_Callback); // 0.3s for sensor measurements
}

//========== CALLBACK FUNCTIONS ==========

static void Clock_Tick_Callback(void) {
    if (HTFT_vGetCurrentPage() == TFT_PAGE_CLOCK) {
        HTFT_vUpdateTime();
    }
}

static void Battery_Tick_Callback(void) {
    if (HTFT_vGetCurrentPage() == TFT_PAGE_BATTERY) {
        HTFT_vUpdateBatteryDisplay();
    }
}

static void Ultrasonic_Display_Callback(void) {
    if (HTFT_vGetCurrentPage() == TFT_PAGE_ULTRASONIC) {
        HTFT_vUpdateUltrasonicDisplay();
    }
}


static void Ultrasonic_Measurement_Callback(void) {
    // Measure distance and update the TFT state
    float distance = 0; //HCSR04_f32GetDistance_cm(&ultrasonic_sensor);
    HTFT_vSetCurrentDistance(distance);

    // Control warning LED based on distance (using PA4 now)
    //if (distance >= 2.0f && distance <= 400.0f) {
      //  if (distance < 10.0f) {
           // MGPIO_vSetPinValue(GPIO_PORTA, PIN4, GPIO_HIGH); // Warning LED ON
       // } else {
            //MGPIO_vSetPinValue(GPIO_PORTA, PIN4, GPIO_LOW);  // Warning LED OFF
       // }
    //}
}

//========== IR REMOTE HANDLER ==========
static void onIrCode(u8 code) {
    switch(code) {
        case IR_CODE_POWER:
            HTFT_vNavigateToPage(TFT_PAGE_FIRST);
            break;
        case IR_CODE_0:
            HTFT_vNavigateToPage(TFT_PAGE_CLOCK);
            break;
        case IR_CODE_1:
            HTFT_vNavigateToPage(TFT_PAGE_BATTERY);
            break;
        case IR_CODE_2:
            HTFT_vNavigateToPage(TFT_PAGE_MODE);
            break;
        case IR_CODE_3:
            HTFT_vNavigateToPage(TFT_PAGE_ULTRASONIC);
            break;
        case IR_CODE_4:
            HTFT_vNavigateToPage(TFT_PAGE_ESP);
            break;
        case IR_CODE_5:
            HTFT_vNavigateToPage(TFT_PAGE_IR_SENSORS);
            break;
        case IR_CODE_6:
            HTFT_vShowSensorsPage(HTFT_vGetCurrentDistance(), 70);
            break;
        case IR_CODE_7:
            HTFT_vDrawImage(image_data_Image, 17120);
            break;
        case IR_CODE_MODE:
            HTFT_vToggleMode();
            break;
        case IR_CODE_LEFT:
        	Motor_FWD(40);
        	break;
        case IR_CODE_EQ:
        	Motor_Left_BWD(30);
        	Motor_Right_FWD(30);
             break;
        case IR_CODE_PLUS:
        	Motor_Left_FWD(30);
        	Motor_Right_BWD(30);
             break;
        case IR_CODE_MINUS:
        	Motor_Stop();
        	break ;
        case IR_CODE_RPT:
        	Motor_BWD(40);
             break;
        default:
            break;
    }
}

//========== MAIN FUNCTION ==========

int main(void) {
    // Initialize all system components
    SystemInit();

    // Set up periodic callbacks
    PeriodicCallbacks_Init();

    // Initialize IR remote control
    IR_Init(onIrCode);

    // Show initial screen
    HTFT_vShowFirstPage();

    // Show startup message
    HTFT_vFillRectangle(10, 80, 200, 95, TFT_COLOR_BLACK);
    HTFT_vDrawText(10, 80, "Initializing WiFi...", TFT_COLOR_YELLOW, TFT_COLOR_BLACK, Font_8x13);


    HTFT_vFillRectangle(10, 80, 200, 95, TFT_COLOR_BLACK);

    // Main application loop
    while(1) {
        // Main loop for non-time-critical tasks


        // Optional: Add any additional background tasks here
        // - Process received WiFi data
        // - Handle other communication protocols
        // - Non-critical sensor readings
    }

    return 0;
}
