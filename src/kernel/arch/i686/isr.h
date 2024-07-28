#pragma once

#include <stdint.h>

typedef struct {
    // Load registers in revers orger they were saved
    uint32_t ds;
    uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
    uint32_t interrupt, error;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed)) Registers;

void i686_ISR_Initialize();