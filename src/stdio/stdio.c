#include "stdio.h"
#include "../vga/vga.h"
#include <stdarg.h>

#define BUFFER_SIZE 32

/* This functions counts placeholders in an unformatted string. */
uint32_t __count_placeholders (uint8_t* text)
{
    uint32_t placeholderCount = 0;
    for (uint32_t i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '%' && text[i + 1] != '%')
        {
            if (i > 0 && text[i - 1] != '\\')
            {
                placeholderCount++;
            }
            else
            {
                placeholderCount++;
            }
        }
    }

    return placeholderCount;
}

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

/* Prints a formatted string into the screen. */
extern void printf (uint8_t* text, ...)
{
    if (text == NULL) return;
    va_list args;
    uint8_t buffer[BUFFER_SIZE];

    // couting the amount of placeholders to init va_list
    uint32_t placeholderCount = __count_placeholders(text);

    if (!placeholderCount) // no need to change anything in this string so far
    {
        // formatting other than placeholder will be done later
        vga_add_text(text);
        return;
    }

    // starting argument list once counted
    va_start(args, placeholderCount);

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
            }

            continue;
        }

        buffer[bufferPos++] = text[i];
    }

    // finally, printing the last stuff placed into the buffer
    vga_add_text(buffer);

    va_end(args);
}
