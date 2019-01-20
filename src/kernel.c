#include "types.h"
#include "vga/vga.h"
#include "stdio/stdio.h"

extern void kernelMain (void* multiboot, uint32_t magic)
{
    vga_init();

    // this first part is to check out if printf is working correctly
    printf("Beggining test...\n");

    for (uint32_t i = 0; i < 50; i++)
    {
        for(uint32_t j = 0; j < i + 1; j++)
        {
            printf("A");
        }

        printf("\n");
    }

    printf("Done testing!\n");

    vga_clear_screen();

    printf("Welcome to Hummingbird OS!\n");
    printf("That's all folks!\n");

    while(1);
}
