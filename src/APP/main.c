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



int batteryLevel = 85;
int batteryVoltage = 12;
int batteryCurrent = 2;
int batteryTemp = 35;

int ultrasonicFront = 45;
int ultrasonicLeft = 120;
int ultrasonicRight = 89;
int ultrasonicRear = 200;

int irLineLeft = 1;
int irLineLeftCenter = 0;
int irLineCenter = 1;
int irLineRightCenter = 1;
int irLineRight = 0;

int envTemp = 28;
int envHumidity = 65;

int motorLeftSpeed = 75;
int motorRightSpeed = 78;
int motorLeftCurrent = 1;
int motorRightCurrent = 1;


char uartBuffer[2048];
void sendRobotData(void) {
    // Clear buffer first
    memset(uartBuffer, 0, sizeof(uartBuffer));

    snprintf(uartBuffer, sizeof(uartBuffer),
        "{"
        "\"battery\":{\"level\":%d,\"voltage\":%d,\"current\":%d,\"temperature\":%d},"
        "\"sensors\":{"
            "\"ultrasonic\":{\"front\":%d,\"left\":%d,\"right\":%d,\"rear\":%d},"
            "\"ir\":{"
                "\"lineLeft\":%d,"
                "\"lineLeftCenter\":%d,"
                "\"lineCenter\":%d,"
                "\"lineRightCenter\":%d,"
                "\"lineRight\":%d"
            "},"
            "\"temperature\":%d,"
            "\"humidity\":%d"
        "},"
        "\"motors\":{\"leftSpeed\":%d,\"rightSpeed\":%d,\"leftCurrent\":%d,\"rightCurrent\":%d},"
        "\"wifi\":{\"connected\":1,\"signal\":-45,\"ip\":\"192.168.1.100\"},"
        "\"mode\":\"autonomous\","
        "\"alerts\":[]"
        "}\r\n", // Add \r\n for proper line termination
        batteryLevel, batteryVoltage, batteryCurrent, batteryTemp,
        ultrasonicFront, ultrasonicLeft, ultrasonicRight, ultrasonicRear,
        irLineLeft, irLineLeftCenter, irLineCenter, irLineRightCenter, irLineRight,
        envTemp, envHumidity,
        motorLeftSpeed, motorRightSpeed, motorLeftCurrent, motorRightCurrent
    );

    MUSART_u8WriteString(USART_PERIPH_6, uartBuffer);
}

void sendTestData(void) {
    snprintf(uartBuffer, sizeof(uartBuffer), "{\"test\":\"hello\",\"value\":123}\n");
    MUSART_u8WriteString(USART_PERIPH_6, uartBuffer);
}
//========== SYSTEM INITIALIZATION ==========

static void SystemInit(void) {
    //---------- RCC Configuration ----------
    MRCC_vInit();
    MRCC_vSetAHBPrescaler(AHB_PRESCALER_DIVIDE_1);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);
    MRCC_vEnableClk(RCC_APB2, RCC_USART6);
    MRCC_vEnableClk(RCC_APB2, RCC_SPI1);

    Motor_Init();


	 GPIOx_PinConfig_t USART2_TX_Pin = {
	        .port = GPIO_PORTA,
	        .pin = PIN11,
	        .mode = GPIO_MODE_ALTFUNC,
	        .speed = GPIO_VHIGH_SPEED,
	        .altFunc = GPIO_AF8_USART6  // AF7 for USART1
	     };
	     MGPIO_vPinInit(&USART2_TX_Pin);

	     GPIOx_PinConfig_t USART2_RX_Pin = {
	             .port = GPIO_PORTA,
	             .pin = PIN12,
	             .mode = GPIO_MODE_ALTFUNC,
	             .speed = GPIO_VHIGH_SPEED,
	             .altFunc = GPIO_AF8_USART6  // AF7 for USART1
	          };
	          MGPIO_vPinInit(&USART2_RX_Pin);

	    USART_Config_t myUsart = {
	        .fclk = USART_CLK_25MHZ ,
	        .peripheral = USART_PERIPH_6,
	        .baudRate = USART_BAUDRATE_115200,
	        .wordLength = USART_WORD_LENGTH_8BITS,
	        .stopBits = USART_STOP_BITS_1,
	        .parity = USART_PARITY_NONE,
	        .sampleRate = USART_SAMPLE_16_TIMES,
	        .sampleMethod = USART_SAMPLE_METHOD_THREE_BITS,
	        .mode = USART_MODE_TX_RX
	     };

	     MUSART_Init(&myUsart);





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
    MSYSTICK_vSetInterval_Multi(4000, sendRobotData);
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
    	//sendTestData();

        // Optional: Add any additional background tasks here
        // - Process received WiFi data
        // - Handle other communication protocols
        // - Non-critical sensor readings
    }

    return 0;
}
