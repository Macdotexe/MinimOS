#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <hal/hal.h>
#include "memory.h"
#include "drivers/kbd.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    clrscr();

    printf("MinimOS Kernel Loaded\n");

    // Test program
    printf("=== Test Program ===\n");
    uint32_t* num1;
    uint32_t* num2;

    printf("Enter number 1: ");
    KBD_ReadNumber(num1);
    printf("Enter number 2: ");
    KBD_ReadNumber(num2);

    printf("Result: %d\n", *num1 + *num2);

end:
    printf("End of program: halted\n");
    for (;;);
}
