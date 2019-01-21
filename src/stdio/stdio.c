#include "stdio.h"
#include "../vga/vga.h"
#include <stdarg.h>

#define BUFFER_SIZE 32

/* This function clears a string writing null characters all over it. */
void __clear_string (uint8_t* str, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) str[i] = '\0';
}

/* This function converts an unsigned int into a string and prints it. */
void __print_unsigned_integer (uint32_t value)
{
    char buffer[BUFFER_SIZE];
    char converted[BUFFER_SIZE] = {'0', '\0'};
    uint32_t bufferPos = 0;
    bool firstNonZeroNumber = false; // this is used to take out all left zeroes

    // taking out each character
    for (uint32_t i = 0; i < 11; i++)
    {
        buffer[bufferPos++] = (value % 10) + '0';
        value /= 10;
    }

    buffer[bufferPos] = '\0';
    bufferPos = 0;

    // reading string backwards, since the number was written backwards
    for (int i = 0; i < 11; i++)
    {
        // ignoring 0's until finding a non 0 value
        if (buffer[10 - i] == '0' && !firstNonZeroNumber) continue;
        firstNonZeroNumber = true;

        converted[bufferPos++] = buffer[10 - i]; // the number itself converted into a string
    }

    // and finally printing it
    vga_add_text(converted);
}

void __print_hex (uint64_t value)
{
    vga_add_text("0x");
    uint8_t* bytes = (uint8_t*) &value; // dividing 8 bytes one by one
    bool firstNonZeroNumber = false;
    uint8_t buffer[BUFFER_SIZE];
    uint8_t bytePos = 0;

    for (int8_t i = sizeof(uint64_t) - 1; i >= 0; i--)
    {
        uint8_t firstNibble = (bytes[i] & 0xF0) >> 4;
        uint8_t lastNibble = bytes[i] & 0x0F;

        uint8_t byte = firstNibble <= 0x9 ? '0' + firstNibble : 'A' + firstNibble - 10;
        if (byte != '0' || firstNonZeroNumber)
        {
            buffer[bytePos++] = byte;
            firstNonZeroNumber = true;
        }

        byte = lastNibble <= 0X9 ? '0' + lastNibble : 'A' + lastNibble - 10;
        if (byte != '0' || firstNonZeroNumber) {
            buffer[bytePos++] = byte;
            firstNonZeroNumber = true;
        }
    }

    if (!firstNonZeroNumber) buffer[bytePos++] = '0';

    buffer[bytePos] = '\0';
    vga_add_text(buffer);
}

/* Prints a formatted string into the screen. */
extern void printf (uint8_t* text, ...)
{
    if (text == NULL) return;
    va_list args;
    uint8_t buffer[BUFFER_SIZE];

    // starting argument list once counted
    va_start(args, text);

    uint32_t bufferPos = 0;
    __clear_string(buffer, BUFFER_SIZE);

    for (uint32_t i = 0; text[i] != '\0'; i++)
    {
        // if buffer is full we print what's in it and release the buffer
        if (bufferPos >= BUFFER_SIZE - 1)
        {
            buffer[BUFFER_SIZE] = '\0';
            vga_add_text(buffer);
            __clear_string(buffer, BUFFER_SIZE);
            bufferPos = 0;
        }

        // finding placeholders
        if (text[i] == '%')
        {
            // printing what is in buffer, because the placeholder will be printed
            vga_add_text(buffer);

            // releasing buffer
            __clear_string(buffer, BUFFER_SIZE);
            bufferPos = 0;

            // dealing with different placeholder types
            switch (text[i + 1])
            {
                case '%':
                    buffer[bufferPos++] = '%';
                    i++;
                    break;
                case 'u':
                    __print_unsigned_integer(va_arg(args, int));
                    i++;
                    break;
                case 'h':
                    __print_hex(va_arg(args, long long int));
                    i++;
                    break;
            }

            continue;
        }

        buffer[bufferPos++] = text[i];
    }

    // finally, printing the last stuff placed into the buffer
    vga_add_text(buffer);

    va_end(args);
}
