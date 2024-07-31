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

    printf("Enter name: ");
    char* name;
    KBD_ReadLine(name);
    printf("name: %s\n", name);

    printf("Enter surname: ");
    char* surname;
    KBD_ReadLine(surname);

    printf("Hello, %s %s!\n", name, surname);

end:
    printf("End of program: halted\n");
    for (;;);
}
