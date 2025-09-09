
/*
 * main.c
 * Test LED Matrix (8x8) using 2 chained 74HC595 chips
 * Author: Mohamed Ismail
 */
#include "Includes_int.h"

u8 Letter_M[8] = {
    0b10000001,
    0b11000011,
    0b10100101,
    0b10011001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001
};

u8 Heart[8] = {
    0b00000000,
    0b01100110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000
};

/**int main(void)
{
	MRCC_vInit();
		MRCC_vEnableClk(RCC_AHB1, GPIO_PORTA);

    LEDMATRIX_vInit();

    while (1)
    {
        LEDMATRIX_vDisplayFrame(Letter_M, 100);
    }
}**/



/*
#include "sound.h"
void DAC_Handler(void);
DAC_Config_t DAC = { .Port = GPIO_PORTA, .pin = { PIN0, PIN1, PIN2, PIN3, PIN4,
		PIN5, PIN6, PIN7 } };

int main(void) {
	MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1, GPIO_PORTA);

	HDAC_vInit(&DAC);
	MSYSTICK_vSetInterval_Multi(391, DAC_Handler);

	while (1) {

	}

}

void DAC_Handler (void)
{

	static u32 iterator = 0;
	HDAC_vSend(&DAC,new_raw, iterator);

	if (iterator < new_raw_len)
	{
		iterator++;
	}else
	{
		iterator = 0;
	}

}*/



/*GPIOx_PinConfig_t tx = {
		.port = GPIO_PORTA,
		.pin = PIN9,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t rx = {
		.port = GPIO_PORTA,
		.pin = PIN10,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t led = {
		.port = GPIO_PORTA,
		.pin = PIN1,
		.mode = GPIO_MODE_OUTPUT,
		.speed = GPIO_LOW_SPEED,
		.outputType = GPIO_PUSHPULL,
		.pull = GPIO_PULLDOWN
};



int main(void){
	MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1,RCC_GPIOA);
	MRCC_vEnableClk(RCC_APB2,RCC_USART1);
	USART_vInit();
	MGPIO_vPinInit(&tx);
	MGPIO_vPinInit(&rx);
	MGPIO_vPinInit(&led);


	USART_vSendData('A');



	if(USART_u8ReiceiveData()=='A'){
		MGPIO_vSetPinValue(GPIO_PORTA,PIN1,GPIO_HIGH);
	}

	while(1){

	}
  return 0;
}

*/
/*
volatile u8 currentNum = 0;

Segment_Init_t SEG1 = {
    .Port = GPIO_PORTA,
    .PinNo = {1,2,3,4,5,6,7,8}   // Example pins, adjust to your wiring
};

Segment_Init_t SEG2 = {
    .Port = GPIO_PORTB,
    .PinNo = {0,1,2,3,4,5,6,7}   // Example pins, adjust to your wiring
};

static void onIrCode(u8 code)
{
    switch(code)
    {
        case IR_CODE_0: currentNum = 0; break;
        case IR_CODE_1: currentNum = 1; break;
        case IR_CODE_2: currentNum = 2; break;
        case IR_CODE_3: currentNum = 3; break;
        case IR_CODE_4: currentNum = 4; break;
        case IR_CODE_5: currentNum = 5; break;
        case IR_CODE_6: currentNum = 6; break;
        case IR_CODE_7: currentNum = 7; break;
        case IR_CODE_8: currentNum = 8; break;
        case IR_CODE_9: currentNum = 9; break;

        case IR_CODE_PLUS:
            currentNum = (currentNum + 1) % 10;
            break;

        case IR_CODE_MINUS:
            currentNum = (currentNum == 0) ? 9 : currentNum - 1;
            break;
    }

    _7_Segment_Write(&SEG1, currentNum);
}

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableClk(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableClk(RCC_AHB1, GPIO_PORTB);
    MRCC_vEnableClk(RCC_APB2, 14);


    _7_Segment_Init(&SEG1);
    _7_Segment_Init(&SEG2);

    MSYSTICK_Config_t stk = {.InterruptEnable = INT_ENABLE, .CLK_SRC = CLK_SRC_AHB_8};
    MSYSTICK_vInit(&stk);

    IR_Init(onIrCode);

    _7_Segment_Write(&SEG1, currentNum);

    while(1) { }
}

*/

/*
#define DELAY_MS(d) do{unsigned int i = d*4000; while(i--){asm("nop");} }while(0)

GPIOx_PinConfig_t tx = {
		.port = GPIO_PORTA,
		.pin = PIN9,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t rx = {
		.port = GPIO_PORTA,
		.pin = PIN10,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t led = {
		.port = GPIO_PORTA,
		.pin = PIN1,
		.mode = GPIO_MODE_OUTPUT,
		.speed = GPIO_LOW_SPEED,
		.outputType = GPIO_PUSHPULL,
		.pull = GPIO_PULLDOWN
};



int main(void){
	MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1,RCC_GPIOA);
	MRCC_vEnableClk(RCC_APB2,RCC_USART1);
	USART_vInit();
	MGPIO_vPinInit(&tx);
	MGPIO_vPinInit(&rx);
	MGPIO_vPinInit(&led);



	while(1){
		USART_vSendData('A');
		USART_vSendData('B');
		USART_vSendData('C');
		USART_vSendData('D');
		USART_vSendData('E');
		USART_vSendData('F');
		DELAY_MS(2500);
		//if(USART_u8ReiceiveData()=='A'){MGPIO_vSetPinValue(GPIO_PORTA,PIN1,GPIO_HIGH);}
	}
  return 0;
}


*/



/*GPIOx_PinConfig_t tx = {
		.port = GPIO_PORTA,
		.pin = PIN9,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t rx = {
		.port = GPIO_PORTA,
		.pin = PIN10,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t led = {
		.port = GPIO_PORTA,
		.pin = PIN1,
		.mode = GPIO_MODE_OUTPUT,
		.speed = GPIO_LOW_SPEED,
		.outputType = GPIO_PUSHPULL,
		.pull = GPIO_PULLDOWN
};



int main(void){
	MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1,RCC_GPIOA);
	MRCC_vEnableClk(RCC_APB2,RCC_USART1);
	USART_vInit();
	MGPIO_vPinInit(&tx);
	MGPIO_vPinInit(&rx);
	MGPIO_vPinInit(&led);


	USART_vSendData('A');



	if(USART_u8ReiceiveData()=='A'){
		MGPIO_vSetPinValue(GPIO_PORTA,PIN1,GPIO_HIGH);
	}

	while(1){

	}
  return 0;
}

*/
/*
volatile u8 currentNum = 0;

Segment_Init_t SEG1 = {
    .Port = GPIO_PORTA,
    .PinNo = {1,2,3,4,5,6,7,8}   // Example pins, adjust to your wiring
};

Segment_Init_t SEG2 = {
    .Port = GPIO_PORTB,
    .PinNo = {0,1,2,3,4,5,6,7}   // Example pins, adjust to your wiring
};

static void onIrCode(u8 code)
{
    switch(code)
    {
        case IR_CODE_0: currentNum = 0; break;
        case IR_CODE_1: currentNum = 1; break;
        case IR_CODE_2: currentNum = 2; break;
        case IR_CODE_3: currentNum = 3; break;
        case IR_CODE_4: currentNum = 4; break;
        case IR_CODE_5: currentNum = 5; break;
        case IR_CODE_6: currentNum = 6; break;
        case IR_CODE_7: currentNum = 7; break;
        case IR_CODE_8: currentNum = 8; break;
        case IR_CODE_9: currentNum = 9; break;

        case IR_CODE_PLUS:
            currentNum = (currentNum + 1) % 10;
            break;

        case IR_CODE_MINUS:
            currentNum = (currentNum == 0) ? 9 : currentNum - 1;
            break;
    }

    _7_Segment_Write(&SEG1, currentNum);
}

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableClk(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableClk(RCC_AHB1, GPIO_PORTB);
    MRCC_vEnableClk(RCC_APB2, 14);


    _7_Segment_Init(&SEG1);
    _7_Segment_Init(&SEG2);

    MSYSTICK_Config_t stk = {.InterruptEnable = INT_ENABLE, .CLK_SRC = CLK_SRC_AHB_8};
    MSYSTICK_vInit(&stk);

    IR_Init(onIrCode);

    _7_Segment_Write(&SEG1, currentNum);

    while(1) { }
}

*/

/*
#define DELAY_MS(d) do{unsigned int i = d*4000; while(i--){asm("nop");} }while(0)

GPIOx_PinConfig_t tx = {
		.port = GPIO_PORTA,
		.pin = PIN9,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t rx = {
		.port = GPIO_PORTA,
		.pin = PIN10,
		.mode = GPIO_MODE_ALTFUNC,
		.altFunc = GPIO_AF7_USART1_USART2
};

GPIOx_PinConfig_t led = {
		.port = GPIO_PORTA,
		.pin = PIN1,
		.mode = GPIO_MODE_OUTPUT,
		.speed = GPIO_LOW_SPEED,
		.outputType = GPIO_PUSHPULL,
		.pull = GPIO_PULLDOWN
};



int main(void){
	MRCC_vInit();
	MRCC_vEnableClk(RCC_AHB1,RCC_GPIOA);
	MRCC_vEnableClk(RCC_APB2,RCC_USART1);
	USART_vInit();
	MGPIO_vPinInit(&tx);
	MGPIO_vPinInit(&rx);
	MGPIO_vPinInit(&led);



	while(1){
		USART_vSendData('A');
		USART_vSendData('B');
		USART_vSendData('C');
		USART_vSendData('D');
		USART_vSendData('E');
		USART_vSendData('F');
		DELAY_MS(2500);
		//if(USART_u8ReiceiveData()=='A'){MGPIO_vSetPinValue(GPIO_PORTA,PIN1,GPIO_HIGH);}
	}
  return 0;
}


*/













