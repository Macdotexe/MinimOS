#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <hal/hal.h>
#include "memory.h"
#include "drivers/kbd.h"
#include "arch/i686/io.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    clrscr();

    printf("MinimOS Kernel Loaded\n");

end:
    for (;;)
    {
        KBD_ReadLine(0);
    }
}
