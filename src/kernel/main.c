#include <stdint.h>
#include <stdio.h>
#include <hal/hal.h>
#include "memory.h"
#include "arch/i686/isr.h"
#include "arch/i686/irq.h"

extern uint8_t __bss_start;
extern uint8_t __end;

uint64_t time;

void timer(Registers* regs)
{
    time++;
}

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    clrscr();

    printf("MinimOS Kernel Loaded\n");

    i686_IRQ_RegisterHandler(0, timer);

end:
    for (;;)
    {
        printf("Time: %u\r", time);
        
    }
}
