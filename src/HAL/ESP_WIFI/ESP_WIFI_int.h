#ifndef HAL_ESP_WIFI_ESP_WIFI_INT_H_
#define HAL_ESP_WIFI_ESP_WIFI_INT_H_

#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_int.h"

typedef enum {
    ESP_OK,
    ESP_ERROR,
    ESP_TIMEOUT,
    ESP_WIFI_NOT_CONNECTED
} ESP_Status_t;

// Initialization
ESP_Status_t ESP_Init(USART_Peripheral_t uart, const char* ssid, const char* password);

// Send data to client
ESP_Status_t ESP_SendString(const char* str);

// Receive data from client (blocking with timeout)
ESP_Status_t ESP_ReceiveString(char* buffer, u32 buffer_size, u32 timeout_ms);

// Check connection status
ESP_Status_t ESP_IsConnected(void);

// Send raw AT command and wait for expected response
ESP_Status_t ESP_SendCommand(USART_Peripheral_t uart, const char* cmd, const char* expected, u32 timeout_ms);


// Start TCP server on port 80
ESP_Status_t ESP_StartHTTPServer(u16 port);

#endif /* HAL_ESP_WIFI_ESP_WIFI_INT_H_ */
