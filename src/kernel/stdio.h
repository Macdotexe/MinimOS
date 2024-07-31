#pragma once
#include <stdint.h>

void clrscr();
void putc(char c);
void movecursor(int up, int down, int left, int right);
void puts(const char* str);
void printf(const char* fmt, ...);
void print_buffer(const char* msg, const void* buffer, uint32_t count);
void backspace();
