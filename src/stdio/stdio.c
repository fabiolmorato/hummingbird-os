#include "stdio.h"
#include "../vga/vga.h"

extern void printf (uint8_t* text)
{
    if (text == NULL) return;
    vga_add_text(text);
}
