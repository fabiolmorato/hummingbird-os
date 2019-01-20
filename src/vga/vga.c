/* VGA Text Mode Library
 * This file is used to manage VGA text mode. The lines are saved into an array
 * of __line_t and dirty lines are updated into the screen. */

#include "vga.h"
#include "../types.h"

__line_t   vga_lines[VGA_HEIGHT]; // lines on the screen
__line_t*  __vga_linesRef[VGA_HEIGHT]; // an array that holds the address of all lines, ordered
// initially every line is ordered 0,1,2,...VGA_HEIGHT-1
// if lines are added at the end it will look like 3,4,5,...VGA_HEIGHT-1,0,1,2
__line_t* __vga_newLinesRef[VGA_HEIGHT]; // when lines are shifted back, this is used
// to write the next state of linesRef

__line_t** vga_linesRef = (__line_t**) __vga_linesRef;
__line_t** vga_newLinesRef = (__line_t**) __vga_newLinesRef;

// video memory
uint16_t* vga_mem = (uint16_t*) 0xB8000;

// these are the positions in which next character will be written
uint32_t vga_x = 0;
uint32_t vga_y = 0;

/* Initializes all structure and control variables. */
void vga_init (void)
{
    for (uint32_t i = 0; i < VGA_HEIGHT; i++)
    {
        vga_linesRef[i] = &vga_lines[i];
        vga_clear_line(i);
    }

    vga_x = 0;
    vga_y = 0;
}

/* Clears the text on a line. */
void vga_clear_line (uint32_t line)
{
    for (uint32_t i = 0; i < VGA_WIDTH; i++) vga_linesRef[line]->text[i] = 0;
    vga_linesRef[line]->dirty = true;
}

/* This function exists for semantics purpose. */
extern void vga_clear_screen (void)
{
    vga_init();
}

/* This function shifts lines back. */
void vga_shift_back_lines (uint32_t amount)
{
    if (amount == 0) return; // nothing to do

    for (uint32_t i = 0; i < VGA_HEIGHT; i++)
    {
        if (i < amount) vga_clear_line(i);
        vga_newLinesRef[i] = vga_linesRef[(i + amount) % VGA_HEIGHT];
        vga_newLinesRef[i]->dirty = true;
        vga_linesRef[i]->dirty = true;
    }

    __line_t** auxLinesRef = (__line_t**) vga_linesRef;
    vga_linesRef = vga_newLinesRef;
    vga_newLinesRef = auxLinesRef;
}

/* Adds a character into current pointer */
void vga_add_char (char c)
{
    // treating newline
    if (c == '\n')
    {
        vga_x = 0;
        vga_y++;
        return;
    }

    // checking if new character will cross boundaries
    if (vga_x >= VGA_WIDTH)
    {
        vga_x = 0;
        vga_y++;
    }

    if (vga_y >= VGA_HEIGHT)
    {
        vga_shift_back_lines(1);
        vga_y = VGA_HEIGHT - 1;
    }

    // adding new character and indicating that line needs to be updated in screen
    vga_linesRef[vga_y]->text[vga_x++] = c;
    vga_linesRef[vga_y]->dirty = true;
}

/* This is used to add some string to the screen. */
void vga_add_text (char* text)
{
    if (text == NULL) return;

    for(uint32_t i = 0; text[i] != '\0'; i++)
    {
        vga_add_char(text[i]);
    }

    vga_update_screen();
}

void vga_update_screen (void)
{
    for (uint32_t i = 0; i < VGA_HEIGHT; i++)
    {
        if (!vga_linesRef[i]->dirty) continue; // line is not dirty, there's no need to update it
        uint8_t* line = (uint8_t*) vga_linesRef[i]->text;

        for (uint32_t j = 0; j < VGA_WIDTH; j++)
        {
            vga_mem[VGA_WIDTH * i + j] = (vga_mem[VGA_WIDTH * i + j] & 0xFF00) | line[j];
        }
    }
}
