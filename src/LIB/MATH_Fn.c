#include "MATH_Fn.h"

// u32 MATH_u32floor(f64 num) {
//     return (u32)num;
// }

// u32 MATH_u32ceil(f64 num) {
//     return (u32)(num + 1);
// }

// f64 MATH_f64GetDecimalPart(f64 num) {
//     f64 decimalPart = num - (f64)MATH_u32floor(num);
//     return decimalPart;
// }

f32 abs(f32 num){
    if (num < 0)
        return -num ;
    else 
        return num ;
}

s32 atoi(const char* str) {
    s32 result = 0;
    s32 sign = 1;

    if (*str == '-') {
        sign = -1;
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result * sign;
}

void itoa(s32 number , u8* buffer ){
    u8 isNegative = 0;
    u8 i = 0;

    if (number < 0) {
        isNegative = 1;
        number = -number;
    }

    do {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    } while (number > 0);

    if (isNegative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    for (u8 j = 0; j < i / 2; j++) {
        u8 temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

void ftoa(f32 number, u8* buffer) {
    // Handle negative numbers
    if (number < 0) {
        *buffer++ = '-';   // add '-' to buffer and advance pointer
        number = -number;  // work with positive equivalent
    }

    // Extract integer part
    int integerPart = (int)number;
    float decimalPart = number - (float)integerPart;

    // Convert integer part
    itoa(integerPart, buffer);

    // Find end of integer string
    int i = 0;
    while (buffer[i] != '\0') {
        i++;
    }

    buffer[i++] = '.';  // add decimal point

    // Convert fractional part (up to 6 digits)
    for (int j = 0; j < 6; j++) {
        decimalPart *= 10.0f;
        int digit = (int)decimalPart;
        buffer[i++] = digit + '0';
        decimalPart -= digit;
    }

    buffer[i] = '\0';
}
