#ifndef __VGA_H
#define __VGA_H

#include "../types.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25

/* This is the structure for all lines in VGA mode.
 * Property text is the text written in that line.
 * Property dirty indicates that it was modified (moved up, a char was added)
 * and that means it needs to be updated. */
typedef struct
{
    uint8_t text[VGA_WIDTH];
    bool dirty;
}
__line_t;

extern void vga_init (void);
void vga_clear_line (uint32_t);
extern void vga_add_text (char*);
extern void vga_clear_screen (void);
void vga_update_screen (void);

#endif
