#include "kbd.h"
#include <stdio.h>
#include "arch/i686/io.h"
#include <string.h>
#include <memory.h>

uint8_t waiting_for_input = 0;
KBD_KEY last_key = NOOP;

void KBD_IRQ(Registers* regs)
{
    uint8_t key = i686_inb(0x60);
    last_key = key;
    if (key > 0x80)
        return;
    
    if (waiting_for_input)
        last_key = key;
        waiting_for_input = 0;

    printf("%c", KBD_Key2Char(key));
}

uint8_t KBD_Key2Char(KBD_KEY key)
{
    switch (key)
    {
        case KEY_ESCAPE: return 0;
        case KEY_1: return '1';
        case KEY_2: return '2';
        case KEY_3: return '3';
        case KEY_4: return '4';
        case KEY_5: return '5';
        case KEY_6: return '6';
        case KEY_7: return '7';
        case KEY_8: return '8';
        case KEY_9: return '9';
        case KEY_0: return '0';
        case KEY_MINUS: return '-';
        case KEY_EQUALS: return '=';
        case KEY_BACKSPACE: backspace(); return NOOP;
        case KEY_TAB: return '\t';
        case KEY_Q: return 'q';
        case KEY_W: return 'w';
        case KEY_E: return 'e';
        case KEY_R: return 'r';
        case KEY_T: return 't';
        case KEY_Y: return 'y';
        case KEY_U: return 'u';
        case KEY_I: return 'i';
        case KEY_O: return 'o';
        case KEY_P: return 'p';
        case KEY_OPEN_BRACKET: return '[';
        case KEY_CLOSE_BRACKET: return ']';
        case KEY_ENTER: return '\n';
        case KEY_A: return 'a';
        case KEY_S: return 's';
        case KEY_D: return 'd';
        case KEY_F: return 'f';
        case KEY_G: return 'g';
        case KEY_H: return 'h';
        case KEY_J: return 'j';
        case KEY_K: return 'k';
        case KEY_L: return 'l';
        case KEY_SEMICOLON: return ';';
        case KEY_SINGLE_QUOTE: return '\'';
        case KEY_BACK_QUOTE: return '`';
        case KEY_Z: return 'z';
        case KEY_X: return 'x';
        case KEY_C: return 'c';
        case KEY_V: return 'v';
        case KEY_B: return 'b';
        case KEY_N: return 'n';
        case KEY_M: return 'm';
        case KEY_COMMA: return ',';
        case KEY_PERIOD: return '.';
        case KEY_FORWARD_SLASH: return '/';
        case KEY_KP_7: return '7';
        case KEY_KP_8: return '8';
        case KEY_KP_9: return '9';
        case KEY_KP_MINUS: return '-';
        case KEY_KP_4: return '4';
        case KEY_KP_5: return '5';
        case KEY_KP_6: return '6';
        case KEY_KP_PLUS: return '+';
        case KEY_KP_1: return '1';
        case KEY_KP_2: return '2';
        case KEY_KP_3: return '3';
        case KEY_KP_0: return '0';
        case KEY_KP_PERIOD: return '.';
        case KEY_SPACE: return ' ';
    }
}

void KBD_ReadLine(char* out)
{
    static char buffer[256] = {0};
    int index = 0;

    while (1)
    {
        waiting_for_input = 1;
        while (waiting_for_input == 1) { }

        switch (last_key)
        {
        case KEY_ENTER:
            goto done;
        case KEY_BACKSPACE:
            if (index > 0)
            {
                index--;
                buffer[index] = '\0';
                movecursor(0, 0, 1, 0);
            }
            break;
        default:
            if (index < sizeof(buffer) - 1)
            {
                buffer[index++] = KBD_Key2Char(last_key);
                buffer[index] = '\0';
            }
            break;
        }
    }

done:
    strcpy(out, buffer);
}

