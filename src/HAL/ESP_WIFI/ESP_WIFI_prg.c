#include "ESP_WIFI_int.h"
#include "../TFT/TFT_int.h"
#include <string.h>
#include <stdio.h>

#define ESP_DEFAULT_TIMEOUT 5000
#define ESP_RETRY_COUNT 5
#define ESP_UART USART_PERIPH_1

static USART_Peripheral_t esp_uart;
extern void DisplayStatus(const char* message, u16 color, u8 line); // Declare external function

// Helper function to clear UART buffer
static void ESP_ClearBuffer(void) {
    u32 timeout = 1000;
    while (timeout-- > 0) {
        if (MUSART_u8ReadByte(esp_uart) == 0) break;
    }
}

// Debug version of receive function
ESP_Status_t ESP_ReceiveString(char* buffer, u32 buffer_size, u32 timeout_ms) {
    u32 start_time = 0; // We'll use simple counter instead
    u32 index = 0;
    u32 no_data_cycles = 0;

    memset(buffer, 0, buffer_size);

    for (u32 cycles = 0; cycles < (timeout_ms / 10); cycles++) {
        u8 c = MUSART_u8ReadByte(esp_uart);

        if (c != 0 && c != 0xFF) { // Valid data received
            if (index < buffer_size - 1) {
                buffer[index++] = c;
                buffer[index] = '\0';
            }
            no_data_cycles = 0;

            // Check for complete response patterns
            if (strstr(buffer, "OK\r\n") || strstr(buffer, "ERROR\r\n") ||
                strstr(buffer, "FAIL\r\n") || strstr(buffer, "ready\r\n") ||
                strstr(buffer, ">\r\n") || strstr(buffer, "+IPD")) {
                return ESP_OK;
            }
        } else {
            no_data_cycles++;
            if (no_data_cycles >= 50) { // 500ms without data
                MSYSTICK_vSetDelay_ms(10);
                no_data_cycles = 0;
            }
        }
    }

    return (index > 0) ? ESP_OK : ESP_TIMEOUT;
}

// Debug version of send command
ESP_Status_t ESP_SendCommand(USART_Peripheral_t uart, const char* cmd, const char* expected, u32 timeout_ms) {
    char buffer[400];
    char debug_msg[50];

    // Clear buffer first
    ESP_ClearBuffer();
    MSYSTICK_vSetDelay_ms(100);

    // Send command
    MUSART_u8WriteString(uart, (u8*)cmd);

    // Debug: Show what command was sent
    snprintf(debug_msg, sizeof(debug_msg), "Sent: %.20s", cmd);
    DisplayStatus(debug_msg, TFT_COLOR_YELLOW, 5);
    MSYSTICK_vSetDelay_ms(200);

    // Wait for response
    ESP_Status_t result = ESP_ReceiveString(buffer, sizeof(buffer), timeout_ms);

    if (result == ESP_OK) {
        // Debug: Show response received
        snprintf(debug_msg, sizeof(debug_msg), "Recv: %.15s", buffer);
        DisplayStatus(debug_msg, TFT_COLOR_CYAN, 6);

        if (strstr(buffer, expected)) {
            return ESP_OK;
        }
        if (strstr(buffer, "ERROR") || strstr(buffer, "FAIL")) {
            DisplayStatus("ESP replied ERROR", TFT_COLOR_RED, 6);
            return ESP_ERROR;
        }
    } else {
        DisplayStatus("No response from ESP", TFT_COLOR_RED, 6);
    }

    return ESP_TIMEOUT;
}

// Enhanced initialization with detailed debugging
ESP_Status_t ESP_Init(USART_Peripheral_t uart, const char* ssid, const char* password) {
    esp_uart = uart;
    char cmd[150];
    u8 retry_count = 0;

    DisplayStatus("Testing ESP comms...", TFT_COLOR_YELLOW, 2);

    // Step 1: Test basic AT communication (most important test)
    for (retry_count = 0; retry_count < 5; retry_count++) {
        DisplayStatus("AT test attempt", TFT_COLOR_YELLOW, 3);

        ESP_Status_t at_result = ESP_SendCommand(uart, "AT\r\n", "OK", 3000);

        if (at_result == ESP_OK) {
            DisplayStatus("AT: OK", TFT_COLOR_GREEN, 3);
            break;
        } else if (at_result == ESP_ERROR) {
            DisplayStatus("AT: ERROR response", TFT_COLOR_RED, 3);
        } else {
            DisplayStatus("AT: No response", TFT_COLOR_RED, 3);
        }

        MSYSTICK_vSetDelay_ms(2000);
    }

    if (retry_count >= 5) {
        DisplayStatus("AT comm failed!", TFT_COLOR_RED, 2);
        return ESP_ERROR;
    }

    // Step 2: Get ESP info
    DisplayStatus("Getting ESP info...", TFT_COLOR_YELLOW, 2);
    ESP_SendCommand(uart, "AT+GMR\r\n", "OK", 3000);
    MSYSTICK_vSetDelay_ms(1000);

    // Step 3: Reset ESP
    DisplayStatus("Resetting ESP...", TFT_COLOR_YELLOW, 2);
    if (ESP_SendCommand(uart, "AT+RST\r\n", "ready", 10000) != ESP_OK) {
        DisplayStatus("Reset failed!", TFT_COLOR_RED, 2);
        // Don't return error, continue anyway
    }
    MSYSTICK_vSetDelay_ms(3000);

    // Step 4: Disable echo
    DisplayStatus("Disable echo...", TFT_COLOR_YELLOW, 2);
    ESP_SendCommand(uart, "ATE0\r\n", "OK", 2000);
    MSYSTICK_vSetDelay_ms(500);

    // Step 5: Set station mode
    DisplayStatus("Set station mode...", TFT_COLOR_YELLOW, 2);
    if (ESP_SendCommand(uart, "AT+CWMODE=1\r\n", "OK", 3000) != ESP_OK) {
        DisplayStatus("Mode set failed!", TFT_COLOR_RED, 2);
        return ESP_ERROR;
    }

    // Step 6: Disconnect from any AP
    DisplayStatus("Disconnect old AP...", TFT_COLOR_YELLOW, 2);
    ESP_SendCommand(uart, "AT+CWQAP\r\n", "OK", 3000);
    MSYSTICK_vSetDelay_ms(1000);

    // Step 7: Connect to WiFi
    DisplayStatus("Connecting WiFi...", TFT_COLOR_YELLOW, 2);

    for (retry_count = 0; retry_count < 3; retry_count++) {
        snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);

        char debug_ssid[30];
        snprintf(debug_ssid, sizeof(debug_ssid), "Try %d: %s", retry_count + 1, ssid);
        DisplayStatus(debug_ssid, TFT_COLOR_CYAN, 3);

        ESP_ClearBuffer();
        MUSART_u8WriteString(uart, (u8*)cmd);

        // Wait up to 25 seconds for connection
        char buffer[300];
        u8 connected = 0;

        for (u32 wait_time = 0; wait_time < 25000; wait_time += 1000) {
            if (ESP_ReceiveString(buffer, sizeof(buffer), 1000) == ESP_OK) {
                if (strstr(buffer, "WIFI CONNECTED") || strstr(buffer, "WIFI GOT IP")) {
                    DisplayStatus("WiFi connected!", TFT_COLOR_GREEN, 3);
                    connected = 1;
                    break;
                }
                if (strstr(buffer, "WIFI DISCONNECT") || strstr(buffer, "FAIL") ||
                    strstr(buffer, "+CWJAP:1") || strstr(buffer, "+CWJAP:2") ||
                    strstr(buffer, "+CWJAP:3") || strstr(buffer, "+CWJAP:4")) {
                    DisplayStatus("WiFi auth failed", TFT_COLOR_RED, 3);
                    break;
                }
            }

            // Show progress
            if ((wait_time % 5000) == 0) {
                char progress[20];
                snprintf(progress, sizeof(progress), "Wait %lus...", wait_time/1000);
                DisplayStatus(progress, TFT_COLOR_YELLOW, 4);
            }
        }

        if (connected) {
            MSYSTICK_vSetDelay_ms(2000);

            // Verify connection and get IP
            DisplayStatus("Getting IP info...", TFT_COLOR_YELLOW, 4);
            ESP_SendCommand(uart, "AT+CIFSR\r\n", "OK", 3000);

            return ESP_OK;
        }

        DisplayStatus("Retry in 3s...", TFT_COLOR_YELLOW, 4);
        MSYSTICK_vSetDelay_ms(3000);
    }

    DisplayStatus("WiFi connect failed!", TFT_COLOR_RED, 2);
    return ESP_WIFI_NOT_CONNECTED;
}

// Rest of the functions remain the same...
ESP_Status_t ESP_SendString(const char* str) {
    char cmd[100];
    u32 len = strlen(str);

    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=0,%lu\r\n", len);

    ESP_ClearBuffer();
    MUSART_u8WriteString(esp_uart, (u8*)cmd);

    char buffer[50];
    u32 timeout = 0;
    u8 ready = 0;

    while (timeout < 5000) {
        if (ESP_ReceiveString(buffer, sizeof(buffer), 100) == ESP_OK) {
            if (strstr(buffer, ">")) {
                ready = 1;
                break;
            }
            if (strstr(buffer, "ERROR") || strstr(buffer, "FAIL")) {
                return ESP_ERROR;
            }
        }
        timeout += 100;
    }

    if (!ready) return ESP_TIMEOUT;

    MUSART_u8WriteString(esp_uart, (u8*)str);
    MSYSTICK_vSetDelay_ms(100);

    return ESP_OK;
}

ESP_Status_t ESP_IsConnected(void) {
    char buffer[200];

    if (ESP_SendCommand(esp_uart, "AT+CWJAP?\r\n", "OK", 3000) == ESP_OK) {
        if (ESP_ReceiveString(buffer, sizeof(buffer), 2000) == ESP_OK) {
            if (strstr(buffer, "No AP") || strstr(buffer, "+CWJAP:1")) {
                return ESP_WIFI_NOT_CONNECTED;
            }
            return ESP_OK;
        }
    }
    return ESP_ERROR;
}

ESP_Status_t ESP_StartHTTPServer(u16 port) {
    char cmd[50];

    DisplayStatus("Enable multi conn...", TFT_COLOR_YELLOW, 4);
    if (ESP_SendCommand(esp_uart, "AT+CIPMUX=1\r\n", "OK", 3000) != ESP_OK) {
        DisplayStatus("CIPMUX failed!", TFT_COLOR_RED, 4);
        return ESP_ERROR;
    }

    DisplayStatus("Start HTTP server...", TFT_COLOR_YELLOW, 4);
    snprintf(cmd, sizeof(cmd), "AT+CIPSERVER=1,%d\r\n", port);
    if (ESP_SendCommand(esp_uart, cmd, "OK", 3000) != ESP_OK) {
        DisplayStatus("Server start failed!", TFT_COLOR_RED, 4);
        return ESP_ERROR;
    }

    return ESP_OK;
}
