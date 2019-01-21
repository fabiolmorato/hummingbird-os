#include "types.h"
#include "vga/vga.h"
#include "stdio/stdio.h"
#include "utils/utils.h"

extern void kernelMain (void* multiboot, uint32_t magic)
{
    vga_init();

    printf("Welcome to Hummingbird OS!\n");
    printf("\nSo far this system can print formatted strings and deals with screen size. As you can see, it correctly treats horizontal boundary.\n");
    printf("\nThe printing system is incomplete. So far you can print:\n\n");
    printf("\t- New lines and tabulations\n");
    printf("\t- Unsigned integers such as %u\n", 3846576);
    printf("\t- Hexadecimal values like %h\n", 0x123456789ABCDEF);
    sleep(300000000 * 4);
    printf("\nAnd also sleep!\n");
    printf("\nEverything implemented so far was made to ease debugging.");
    printf("\n\nThat's all folks!\n");

    while(1);
}
