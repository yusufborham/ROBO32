/*
 * old_mains.c
 *
 *  Created on: Aug 27, 2025
 *      Author: Koshok
 */



/*
void LED0(void);
void LED1(void);
void LED2(void);


int main(void)
{

	MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
	GPIOx_PinConfig_t LED[3] ={
			{GPIO_PORTA, PIN0, GPIO_MODE_OUTPUT, GPIO_PUSHPULL, GPIO_LOW_SPEED, GPIO_NOPULL,0},
			{GPIO_PORTA, PIN1, GPIO_MODE_OUTPUT, GPIO_PUSHPULL, GPIO_LOW_SPEED, GPIO_NOPULL,0},
			{GPIO_PORTA, PIN2, GPIO_MODE_OUTPUT, GPIO_PUSHPULL, GPIO_LOW_SPEED, GPIO_NOPULL,0}

	};
	for(u8 i=0;i<3;i++)
		MGPIO_vPinInit(&LED[i]);


	OS_CreateTask(LED0, 50, 0, 0);
	OS_CreateTask(LED1, 100, 1, 1);
	OS_CreateTask(LED2, 10, 2, 2);

	OS_vStart();
	while(1)
	{

	}

	return 0 ;
}

void LED0(void)
{
	MGPIO_vTogPinValue(GPIO_PORTA, PIN0);
}
void LED1(void)
{
	MGPIO_vTogPinValue(GPIO_PORTA, PIN1);
}
void LED2(void)
{
	MGPIO_vTogPinValue(GPIO_PORTA, PIN2);
}
*/




/*
	int main(void)
	{
	    // Init RCC and GPIO clocks first
	    MRCC_vInit();
	    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);
	    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);

	    LEDMATRIX_Config_t Matrix = {
	        .RowPort = GPIO_PORTA,
	        .RowPins = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7},

	        .ColPort = GPIO_PORTB,
	        .ColPins = {PIN0, PIN1, PIN2, PIN5, PIN6, PIN7, PIN8, PIN9}
	    };

	    HLEDMATRIX_vInit(&Matrix);

	    // A simple pattern: diagonal line
	    u8 frame1[8] = {
			0b10000001,
			0b10000001,
			0b10000001,
			0b11111111,
			0b10000001,
			0b10000001,
			0b10000001,
			0b00000000
			};

	    // Another pattern: full square
	    u8 frame2[8] = {
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b11111000,
			0b10001000,
			0b10001000,
			0b00000000
	    };

	    while (1)
	    {
	        // Show diagonal for ~500ms
	    	HLEDMATRIX_vDisplayFrame(&Matrix,frame1, 50);

	        // Show square for ~500ms
	    	HLEDMATRIX_vDisplayFrame(&Matrix,frame2, 50);
	    }
	}

*/
// btn Increment and decrment

/*volatile s8 counter = 0;



void IncrementBtn_Handler(void)
{
    counter++;
    if (counter > 9) counter = 0;

    MEXTI_vClearPendingFlag(EXTI_LINE0);
}

void DecrementBtn_Handler(void)
{
    counter--;
    if (counter < 0) counter = 9;

    MEXTI_vClearPendingFlag(EXTI_LINE1);
}


int main(void)
{

    MRCC_vInit();
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);   // For buttons
    MRCC_vEnableClk(RCC_AHB1, RCC_GPIOB);   // For 7-segment
    MRCC_vEnableClk(RCC_APB2, RCC_SYSCFG);  // Enable SYSCFG clock for EXTI


    Segment_Init_t SEG1 = {
        .Port = GPIO_PORTB,
        .PinNo = {PIN0, PIN1, PIN2, PIN5, PIN6, PIN7, PIN8, PIN9}
    };
    _7_Segment_Init(&SEG1);
    _7_Segment_Write(&SEG1, counter);

    GPIOx_PinConfig_t btn1 = {
        .port = GPIO_PORTA,
        .pin = PIN0,
        .mode = GPIO_MODE_INPUT,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_LOW_SPEED,
        .pull = GPIO_PULLDOWN,
        .altFunc = 0
    };
    MGPIO_vPinInit(&btn1);

    GPIOx_PinConfig_t btn2 = {
        .port = GPIO_PORTA,
        .pin = PIN1,
        .mode = GPIO_MODE_INPUT,
        .outputType = GPIO_PUSHPULL,
        .speed = GPIO_LOW_SPEED,
        .pull = GPIO_PULLDOWN,
        .altFunc = 0
    };
    MGPIO_vPinInit(&btn2);


    MSYSCFG_vSetEXTIPort(EXTI_LINE0, SYSCFG_PORTA);
    MSYSCFG_vSetEXTIPort(EXTI_LINE1, SYSCFG_PORTA);

    MEXTI_vEnableINT(EXTI_LINE0, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(IncrementBtn_Handler, EXTI_LINE0);
    MNVIC_vEnable_Peripheral_INT(6);   // EXTI0 IRQn

    MEXTI_vEnableINT(EXTI_LINE1, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(DecrementBtn_Handler, EXTI_LINE1);
    MNVIC_vEnable_Peripheral_INT(7);   // EXTI1 IRQn

    while (1)
    {
        _7_Segment_Write(&SEG1, counter);
    }

    return 0;
}

*/

/*MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1, RCC_GPIOA);

	MSYSTICK_Config_t STK_cfg = {.InterruptEnable = INT_DISABLE , .CLK_SRC = CLK_SRC_AHB_8};
	MSYSTICK_vInit(& STK_cfg);

	GPIOx_PinConfig_t led = {.port = GPIO_PORTA,
	        .pin = PIN1,
	        .mode = GPIO_MODE_OUTPUT,
	        .outputType = GPIO_PUSHPULL,
	        .speed = GPIO_LOW_SPEED,
	        .pull = GPIO_NOPULL,
	        .altFunc = 0};
	MGPIO_vPinInit(&led);

	while(1){
		MGPIO_vSetPinValue(GPIO_PORTA, PIN1, GPIO_HIGH);
		MSYSTICK_vSetDelay_ms(1000);
		MGPIO_vSetPinValue(GPIO_PORTA, PIN1, GPIO_LOW);
		MSYSTICK_vSetDelay_ms(1000);
	}*/




// led matrix main





