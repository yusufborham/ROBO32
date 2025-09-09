#include "../NVIC/NVIC_int.h"
#include "../SYSTICK/SYSTICK_int.h"
#include "../USART/USART_cfg.h"
#include "../USART/USART_int.h"
#include "../USART/USART_prv.h"


static USART_parsingIntStatus_t G_aUSART_parsingStates[USART_NUMBER] = {{0}};
static USART_Buffer_t G_aUSART_Buffer_data[USART_NUMBER] = {{0}};
static USART_InterruptStatus_t G_aUSART_InterruptStatus[USART_NUMBER] = {{0}};

USART_Status_t USART_vHandlerRoutine(USART_Peripheral_t thisID);
void MUSART_vBaudRateCalculations(u32 A_u32BaudRateValue ,u8 A_u8SampleRate ,u32 A_u32USART_Fclk,USART_BaudRate_cfg_t* A_sBaudRateCfg);
static inline USART_RegDef_t* MUSART_pGetRegDef(USART_Peripheral_t peripheral) ;
static inline u8 MUSART_GetFlagStatus(USART_Peripheral_t peripheral ,USART_Flags_t flag) ;
static inline USART_Status_t MUSART_DisableInterrupt(USART_Peripheral_t peripheral, USART_Interrupts_t interrupt);
static inline USART_Status_t MUSART_EnableInterrupt(USART_Peripheral_t peripheral,USART_Interrupts_t interrupt);
static inline void MUSART_vEnableNVIC(USART_Peripheral_t peripheral);
static inline void MUSART_vDisableNVIC(USART_Peripheral_t peripheral) ;// A single, private helper to rule them all


static inline USART_RegDef_t* MUSART_pGetRegDef(USART_Peripheral_t peripheral) {
    if (peripheral == USART_PERIPH_1) return USART1;
    if (peripheral == USART_PERIPH_2) return USART2;
    if (peripheral == USART_PERIPH_6) return USART6;
    return NULL;
}

static inline u8 MUSART_GetFlagStatus(USART_Peripheral_t peripheral, USART_Flags_t flag){
    USART_RegDef_t* USARTx = MUSART_pGetRegDef(peripheral);
    if (USARTx == NULL) return 0;

    return (USARTx->SR & (1 << flag)) ? 1 : 0;
}

static inline USART_Status_t MUSART_EnableInterrupt(USART_Peripheral_t peripheral , USART_Interrupts_t interrupt){
    USART_RegDef_t* USARTx = MUSART_pGetRegDef(peripheral);
    if (USARTx == NULL) return USART_ERR_BAD_PERIPH;
    SET_BIT(USARTx->CR1, interrupt);
    return USART_OK;
}

static inline USART_Status_t MUSART_DisableInterrupt(USART_Peripheral_t peripheral, USART_Interrupts_t interrupt) {
    USART_RegDef_t* USARTx = MUSART_pGetRegDef(peripheral);
    if (USARTx == NULL) return USART_ERR_BAD_PERIPH;
    CLR_BIT(USARTx->CR1, interrupt);
    return USART_OK;
}

void MUSART_vBaudRateCalculations(u32 A_u32BaudRateValue ,u8 A_u8SampleRate ,u32 A_u32USART_Fclk,USART_BaudRate_cfg_t* A_sBaudRateCfg){
    A_sBaudRateCfg->DIV_Mantissa = (A_u32USART_Fclk) / (A_u8SampleRate * A_u32BaudRateValue);
    u8 A_u8Fraction = (u8) (((A_u32USART_Fclk) % (A_u8SampleRate * A_u32BaudRateValue) * A_u8SampleRate) / (A_u8SampleRate * A_u32BaudRateValue));
    A_sBaudRateCfg->DIV_Fraction = (A_u8SampleRate == 8 ) ? (A_u8Fraction & 0x07) : A_u8Fraction;
}

static inline void MUSART_vEnableNVIC(USART_Peripheral_t peripheral){
    switch (peripheral) {
        case USART_PERIPH_1:
            MNVIC_vEnableInterrupt(37);
            break;
        case USART_PERIPH_2:
            MNVIC_vEnableInterrupt(38);
            break;
        case USART_PERIPH_6:
            MNVIC_vEnableInterrupt(71);
            break;
        default:
            // Invalid peripheral, do nothing or handle error
            break;
    }
}

static inline void MUSART_vDisableNVIC(USART_Peripheral_t peripheral){
    switch (peripheral) {
        case USART_PERIPH_1:
            MNVIC_vDisableInterrupt(37);
            break;
        case USART_PERIPH_2:
            MNVIC_vDisableInterrupt(38);
            break;
        case USART_PERIPH_6:
            MNVIC_vDisableInterrupt(71);
            break;
        default:
            // Invalid peripheral, do nothing or handle error
            break;
    }
}



USART_Status_t MUSART_Init(USART_Config_t* cfg){
    // ############# Future Note ###############
    // I must enable the interrupt from the NVIC driver also //
    // maybe set a priorty also ///
    if (cfg == NULL) return USART_ERR_NULLCFG;

    u8 L_u8USART_SampleRate = DEFAULT_SAMPLE_RATE ;
    // set default values if the user has not provided them
    if (cfg->sampleRate == 0) cfg->sampleRate = USART_SAMPLE_16_TIMES;
    if (cfg->sampleMethod == 0) cfg->sampleMethod = USART_SAMPLE_METHOD_THREE_BITS;
    if (cfg->mode == 0) cfg->mode = USART_MODE_TX_RX;
    if (cfg->parity == 0) cfg->parity = USART_PARITY_NONE;
    if (cfg->wordLength == 0) cfg->wordLength = USART_WORD_LENGTH_8BITS;
    if (cfg->stopBits == 0) cfg->stopBits = USART_STOP_BITS_1;

    u32 L_u32USART_Fclk = MEGA(cfg->fclk);

    USART_RegDef_t* USARTx = MUSART_pGetRegDef(cfg->peripheral);
    if (USARTx == NULL) return USART_ERR_BAD_PERIPH;

    MUSART_vEnableNVIC(cfg->peripheral);

    MSYSTICK_vEnableBackgroundMillis();
    // ENABLE USART
    SET_BIT(USARTx->CR1 , UE);

    switch (cfg->wordLength){
        case USART_WORD_LENGTH_8BITS:
            CLR_BIT(USARTx->CR1 , M);
            break;
        case USART_WORD_LENGTH_9BITS:
            SET_BIT(USARTx->CR1 , M);
            break;
    }
    switch (cfg->stopBits){
        case USART_STOP_BITS_1:
            USARTx->CR2 &= ~(USART_CR2_STOP_Msk << STOP);
            USARTx->CR2 |= (USART_1_STOP_BIT << STOP);
            break;
        case USART_STOP_BITS_1_5:
            USARTx->CR2 &= ~(USART_CR2_STOP_Msk << STOP);
            USARTx->CR2 |= (USART_1_5_STOP_BIT << STOP);
            break;
        case USART_STOP_BITS_2:
            USARTx->CR2 &= ~(USART_CR2_STOP_Msk << STOP);
            USARTx->CR2 |= (USART_2_STOP_BIT << STOP);
            break;
        case USART_STOP_BITS_0_5:
            USARTx->CR2 &= ~(USART_CR2_STOP_Msk << STOP);
            USARTx->CR2 |= (USART_0_5_STOP_BIT << STOP);
            break;
    }
    switch (cfg->parity){
        case USART_PARITY_NONE:
            CLR_BIT(USARTx->CR1 , PCE);
            break;
        case USART_PARITY_EVEN:
            SET_BIT(USARTx->CR1 , PCE);
            CLR_BIT(USARTx->CR1 , PS);
            break;
        case USART_PARITY_ODD:
            SET_BIT(USARTx->CR1 , PCE);
            SET_BIT(USARTx->CR1 , PS);
            break;
    }
    switch (cfg->sampleRate){
        case USART_SAMPLE_16_TIMES:
            CLR_BIT(USARTx->CR1 , OVER8);
            L_u8USART_SampleRate = 16 ;
            break;
        case USART_SAMPLE_8_TIMES:
            SET_BIT(USARTx->CR1 , OVER8);
            L_u8USART_SampleRate = 8 ;
            break;
    }
    switch (cfg->sampleMethod){
        case USART_SAMPLE_METHOD_ONE_BIT:
            SET_BIT(USARTx->CR3 , ONEBIT);
            break;
        case USART_SAMPLE_METHOD_THREE_BITS:
            CLR_BIT(USARTx->CR3 , ONEBIT);
            break;
    }
    switch (cfg->mode){
        case USART_MODE_TX_ONLY:
            SET_BIT(USARTx->CR1 , TE);
            CLR_BIT(USARTx->CR1 , RE);
            break;
        case USART_MODE_RX_ONLY:
            SET_BIT(USARTx->CR1 , RE);
            CLR_BIT(USARTx->CR1 , TE);
            break;
        case USART_MODE_TX_RX:
            SET_BIT(USARTx->CR1 , TE);
            SET_BIT(USARTx->CR1 , RE);
            break;
    }
    MUSART_EnableInterrupt(cfg->peripheral , USART_READ_DATA_READY_INT);
    USART_BaudRate_cfg_t L_sU1BaudRateCfg;
    MUSART_vBaudRateCalculations(cfg->baudRate , L_u8USART_SampleRate , L_u32USART_Fclk, &L_sU1BaudRateCfg);
    USARTx->BRR = (L_sU1BaudRateCfg.DIV_Mantissa << 4) | (L_sU1BaudRateCfg.DIV_Fraction & 0x0F);

    return USART_OK;
}


u8 MUSART_u8ReadByte(USART_Peripheral_t A_thisID){
    if (A_thisID < USART_PERIPH_1 || A_thisID > USART_PERIPH_6) return USART_ERR_BAD_PERIPH ;

    USART_Buffer_t* L_spUSART_buffData = &G_aUSART_Buffer_data[A_thisID] ;
    u8 L_u8readData = 0 ;

    if (L_spUSART_buffData->rxGetPtr == L_spUSART_buffData->rxPutPtr )
        return 0 ;

    else {
        MUSART_vDisableNVIC(A_thisID);
         // critical section: disable RXNE interrupt only
        L_u8readData = L_spUSART_buffData->USART_RX_BUFFER[L_spUSART_buffData->rxGetPtr++];
        L_spUSART_buffData->rxGetPtr =(L_spUSART_buffData->rxGetPtr ) % MAX_RX_BUFFER_SIZE ;
        MUSART_vEnableNVIC(A_thisID);
         // re-enable RXNE interrupt
        return L_u8readData ;
    }
}
u8 MUSART_u8BytesAvailable(USART_Peripheral_t A_thisID){
    if (A_thisID < USART_PERIPH_1 || A_thisID > USART_PERIPH_6) return USART_ERR_BAD_PERIPH ;
    USART_Buffer_t* L_spUSART_buffData = &G_aUSART_Buffer_data[A_thisID] ;

    return (L_spUSART_buffData->rxPutPtr - L_spUSART_buffData->rxGetPtr + MAX_RX_BUFFER_SIZE) % MAX_RX_BUFFER_SIZE;
}

USART_Status_t MUSART_u8WriteByte(USART_Peripheral_t A_thisID , u8 A_u8ByteToPush) {
    if (A_thisID < USART_PERIPH_1 || A_thisID > USART_PERIPH_6) return USART_ERR_BAD_PERIPH;
    if (A_u8ByteToPush == '\0') return USART_OK; // ignore null char

    USART_Buffer_t* L_spUSART_buffData = &G_aUSART_Buffer_data[A_thisID];

    u16 nextPtr = (L_spUSART_buffData->txPutPtr + 1) % MAX_TX_BUFFER_SIZE;
    if (nextPtr == L_spUSART_buffData->txGetPtr) {
        return USART_ERR_BUFFER_FULL; // no space
    }

    // critical section: disable TXE interrupt only
    MUSART_vDisableNVIC(A_thisID);

    L_spUSART_buffData->USART_TX_BUFFER[L_spUSART_buffData->txPutPtr] = A_u8ByteToPush;
    L_spUSART_buffData->txPutPtr = nextPtr;

    MUSART_EnableInterrupt(A_thisID, USART_TRANSMIT_DATA_REG_EMPTY_INT);
    MUSART_vEnableNVIC(A_thisID);

    G_aUSART_InterruptStatus[A_thisID].Tx_enabled = 1;

    return USART_OK;
}


USART_Status_t MUSART_u8WriteString(USART_Peripheral_t A_thisID ,const u8* A_u8StringToPush){
    while (*A_u8StringToPush != '\0'){
    	USART_Status_t status = MUSART_u8WriteByte(A_thisID , *(A_u8StringToPush++)) ;
        if (status != USART_OK) {
            if (status == USART_ERR_BUFFER_FULL ) {
                #if OVERWRITE_OLD_DATA_TX
                    continue;
                #else
                    return USART_ERR_BUFFER_FULL ;
                #endif
            }
            else
                return status ;
        }
    }
    return USART_OK ;
}

USART_Status_t MUSART_u8ReadStringUntil(USART_Peripheral_t A_thisID, u8 *A_u8pStringBuffer,u32 A_u32BufferSize ,u8 A_u8TerminatingChar){
    static u32 idx = 0 ;
    while (MUSART_u8BytesAvailable(A_thisID)){
       A_u8pStringBuffer[idx] = MUSART_u8ReadByte(A_thisID) ;
       if ( (A_u8pStringBuffer[idx] == A_u8TerminatingChar) || idx >= A_u32BufferSize - 1 ){
            A_u8pStringBuffer[idx] = '\0' ;
            idx = 0 ;
            if (idx >= A_u32BufferSize - 1)
                return USART_STRING_BUFFER_OVF ;
            break ;
       }
       idx++;
    }
    return USART_OK  ;
}

USART_Status_t MUSART_u32ParseIntBlocking(USART_Peripheral_t A_thisID,  s32* A_ps32Result,  u32 timeout_ms) {

    if (A_thisID < USART_PERIPH_1 || A_thisID > USART_PERIPH_6) return USART_ERR_BAD_PERIPH ;
    if (A_ps32Result == NULL) return INVALID_ARGUMENT ;

    USART_parsingIntStatus_t* L_spUSART_parsingStatus = &G_aUSART_parsingStates[A_thisID] ;


    L_spUSART_parsingStatus->result = 0;
    L_spUSART_parsingStatus->sign = 1;
    L_spUSART_parsingStatus->isFirstByte = 1;

    // Scale timeout to something based on your system tick
    L_spUSART_parsingStatus->entryTime = MSYSTICK_millis(); // if you treat loop ~1�s

    while (MSYSTICK_millis() - L_spUSART_parsingStatus->entryTime < timeout_ms) {
        if (!MUSART_u8BytesAvailable(A_thisID))
            continue; // wait until data arrives

        u8 byte = MUSART_u8ReadByte(A_thisID);

        if (L_spUSART_parsingStatus->isFirstByte) {
            if (byte == '-') {
                L_spUSART_parsingStatus->sign = -1;
                L_spUSART_parsingStatus->isFirstByte = 0;
                continue;
            } else if (byte >= '0' && byte <= '9') {
                L_spUSART_parsingStatus->result = (byte - '0');
                L_spUSART_parsingStatus->isFirstByte = 0;
                continue;
            } else {
                // Ignore other leading chars
                continue;
            }
        }

        if (byte >= '0' && byte <= '9') {
            L_spUSART_parsingStatus->result = L_spUSART_parsingStatus->result * 10 + (byte - '0');
        } else {
            break; // stop on non-digit
        }
    }
    if ((MSYSTICK_millis() - L_spUSART_parsingStatus->entryTime + timeout_ms ) >= 0)
        return USART_ERR_TIMEOUT; // timeout occurred

    *A_ps32Result = L_spUSART_parsingStatus->result * L_spUSART_parsingStatus->sign;

    return USART_OK ;
}

USART_ParsingStatus_t MUSART_u8ParseInt(USART_Peripheral_t A_thisID , s32* A_ps32Result) {
    if (A_thisID < USART_PERIPH_1 || A_thisID > USART_PERIPH_6) return USART_ERR_BAD_PERIPH ;

    if (A_ps32Result == NULL) return INVALID_ARGUMENT ;

    USART_parsingIntStatus_t* L_spUSART_parsingStatus = &G_aUSART_parsingStates[A_thisID] ;

    while (MUSART_u8BytesAvailable(A_thisID)) {
        u8 byte = MUSART_u8ReadByte(A_thisID);

        // Handle first byte (sign or digit)
        if (L_spUSART_parsingStatus->isFirstByte) {
            L_spUSART_parsingStatus->isParsing = 1;
            if (byte == '-') {
                L_spUSART_parsingStatus->sign = -1;
                L_spUSART_parsingStatus->isFirstByte = 0;
                continue;
            } else if (byte >= '0' && byte <= '9') {
                L_spUSART_parsingStatus->result = (byte - '0');
                L_spUSART_parsingStatus->isFirstByte = 0;
                continue;
            } else {
                // Ignore garbage until valid number starts
                continue;
            }
        }

        // Handle digits
        if (byte >= '0' && byte <= '9') {
            L_spUSART_parsingStatus->result = L_spUSART_parsingStatus->result * 10 + (byte - '0');
        } else {
            // End of number detected
            *A_ps32Result = L_spUSART_parsingStatus->result * L_spUSART_parsingStatus->sign;

            // Reset state for next call
            L_spUSART_parsingStatus->result = 0;
            L_spUSART_parsingStatus->isFirstByte = 1;
            L_spUSART_parsingStatus->sign = 1;
            L_spUSART_parsingStatus->isParsing = 0;
            return DONE_PARSING ;
        }
    }

    // No full number yet
    return (L_spUSART_parsingStatus->isParsing ? STILL_PARSING : DONE_PARSING);
}

// f32 MUSART_f32ParseFloatBlocking(USART_Peripheral_t A_thisID, u32 timeout_ms) {
//     f32 result = 0.0;
//     u8 isFirstByte = 1;
//     s8 sign = 1;
//     f32 decimalFactor = 0.1;
//     u8 inDecimalPart = 0;

//     // Scale timeout to something based on your system tick
//     u32 countdown = timeout_ms * 1000; // if you treat loop ~1�s

//     while (countdown--) {
//         if (!MUSART_u8BytesAvailable(A_thisID))
//             continue; // wait until data arrives

//         u8 byte = MUSART_u8ReadByte(A_thisID);

//         if (isFirstByte) {
//             if (byte == '-') {
//                 sign = -1;
//                 isFirstByte = 0;
//                 continue;
//             } else if (byte >= '0' && byte <= '9') {
//                 result = (byte - '0');
//                 isFirstByte = 0;
//                 continue;
//             } else {
//                 // Ignore other leading chars
//                 continue;
//             }
//         }

//         if (byte >= '0' && byte <= '9') {
//             if (inDecimalPart) {
//                 result += (byte - '0') * decimalFactor;
//                 decimalFactor *= 0.1;
//             } else {
//                 result = result * 10 + (byte - '0');
//             }
//         } else if (byte == '.' && !inDecimalPart) {
//             inDecimalPart = 1; // start processing decimal part
//         } else {
//             break; // stop on non-digit/non-decimal point
//         }
//     }

//     return result * sign;
// }

USART_Status_t MUSART_vFlush(USART_Peripheral_t A_thisID){

     if (A_thisID < USART_PERIPH_1 || A_thisID > USART_PERIPH_6) return USART_ERR_BAD_PERIPH ;
    USART_Buffer_t* L_spUSART_buffData = &G_aUSART_Buffer_data[A_thisID] ;

    MUSART_DisableInterrupt(A_thisID ,  USART_READ_DATA_READY_INT);
    L_spUSART_buffData->rxGetPtr = L_spUSART_buffData->rxPutPtr ;
    MUSART_EnableInterrupt(A_thisID ,  USART_READ_DATA_READY_INT);
    return USART_OK ;

}

USART_Status_t USART_vHandlerRoutine(USART_Peripheral_t thisID){

    USART_RegDef_t* USARTx = NULL;

    switch (thisID) {
        case USART_PERIPH_1:
            USARTx = USART1;
            break;
        case USART_PERIPH_2:
            USARTx = USART2;
            break;
        case USART_PERIPH_6:
            USARTx = USART6;
            break;
        default:
            return USART_ERR_BAD_PERIPH;
    }

     if (MUSART_GetFlagStatus(thisID , USART_FLAG_READ_DATA_READY)){                   // if there is data to read
        G_aUSART_Buffer_data[thisID].USART_RX_BUFFER[G_aUSART_Buffer_data[thisID].rxPutPtr] = USARTx->DR  ;       // reading from the data register clears the flag
        G_aUSART_Buffer_data[thisID].rxPutPtr++ ;
        G_aUSART_Buffer_data[thisID].rxPutPtr = G_aUSART_Buffer_data[thisID].rxPutPtr % MAX_RX_BUFFER_SIZE ;       // constrain to the range of the buffer
    }

    if (MUSART_GetFlagStatus(thisID , USART_FLAG_TRANSMIT_DATA_REG_EMPTY)){
        if (G_aUSART_Buffer_data[thisID].txPutPtr == G_aUSART_Buffer_data[thisID].txGetPtr )
        {
            // if no new data to send disable the interrupt
            MUSART_DisableInterrupt(thisID , USART_TRANSMIT_DATA_REG_EMPTY_INT) ;
            G_aUSART_InterruptStatus[thisID].Tx_enabled = 0 ;
        }

        else {
            USARTx->DR = G_aUSART_Buffer_data[thisID].USART_TX_BUFFER[G_aUSART_Buffer_data[thisID].txGetPtr++] ;
            G_aUSART_Buffer_data[thisID].txGetPtr = (G_aUSART_Buffer_data[thisID].txGetPtr) % MAX_TX_BUFFER_SIZE ;
        }
    }
    return USART_OK ;
}

void USART1_IRQHandler(void){
    USART_vHandlerRoutine(USART_PERIPH_1) ;
}

void USART2_IRQHandler(void){
    USART_vHandlerRoutine(USART_PERIPH_2) ;
}

void USART6_IRQHandler(void){
    USART_vHandlerRoutine(USART_PERIPH_6) ;
}
