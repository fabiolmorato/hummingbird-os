#include "utils.h"
#include "../types.h"

extern void sleep (uint32_t cycles)
{
    for (uint32_t i = 0; i < cycles; i++) asm("nop");
}
