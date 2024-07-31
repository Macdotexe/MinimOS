#include "hal.h"
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>
#include <arch/i686/isr.h>
#include <arch/i686/irq.h>
#include <drivers/kbd.h>

static void ignore() {}

void HAL_Initialize()
{
    i686_GDT_Initialize();
    i686_IDT_Initialize();
    i686_ISR_Initialize();
    i686_IRQ_Initialize();

    // Register IRQ handlers
    i686_IRQ_RegisterHandler(0, ignore);
    i686_IRQ_RegisterHandler(1, KBD_IRQ);
}
