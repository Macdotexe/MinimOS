#include "kbd.h"
#include <stdio.h>
#include "arch/i686/io.h"
#include <string.h>
#include <memory.h>

uint8_t waiting_for_input = 0;
KBD_KEY last_press = NOOP;

uint8_t caps_lock = 0;
uint8_t shift = 0;
uint8_t ctrl = 0;

uint8_t printing = 0;
uint8_t print_counter = 0;

void KBD_IRQ(Registers* regs)
{
    uint8_t key = i686_inb(0x60);
    
    if (KBD_Key2Char(key) != 0)
    {
        last_press = key;
        waiting_for_input = 0;
    }
    

    switch (key)
    {
    case KEY_LEFT_SHIFT:
        shift = 1;
        return;
    case KEY_LEFT_CONTROL:
        ctrl = 1;
        return;
    case KEY_CAPS_LOCK:
        caps_lock = !caps_lock;
        return;
    case KEY_LEFT_SHIFT_RELEASED:
        shift = 0;
        return;
    case KEY_LEFT_CONTROL_RELEASED:
        ctrl = 0;
        return;
    }
    

    if (printing == 1)
    {
        if (key == KEY_BACKSPACE)
        {
            if (print_counter > 0)
            {
                backspace();
                print_counter--;
            }
            
        }
        else
        {
            printf("%c", KBD_Key2Char(key));
            print_counter++;
        }
        
        
    }
    
}

uint8_t KBD_Key2Char(KBD_KEY key)
{
    switch (key)
    {
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
        default: return 0;
    }
}

void KBD_ReadLine(char* out)
{
    static char buffer[256] = {0};
    int index = 0;

    // reset buffer
    memset(buffer, 0, sizeof(buffer));

    // set up for printing
    printing = 1;
    print_counter = 0;

    while (1)
    {
        // tell IRQ handler to notify on input
        waiting_for_input = 1;
        while (waiting_for_input == 1) { }

        switch (last_press)
        {
        case KEY_ENTER:
            goto done;
        case KEY_BACKSPACE:
            if (index > 0)
            {
                index--;
                buffer[index] = '\0';
            }
            break;
        default:
            if (index < sizeof(buffer) - 1)
            {
                buffer[index++] = KBD_Key2Char(last_press);
                buffer[index] = '\0';
            }
            break;
        }

        // printf("\nBuffer: %s\n", buffer);
    }

done:
    strcpy(out, buffer);
}


void KBD_ReadNumber(uint32_t* out)
{
    static char buffer[256] = {0};
    int index = 0;
    int valid_input = 0;

    // reset buffer
    memset(buffer, 0, sizeof(buffer));

    while (1)
    {
        // tell IRQ handler to notify on input
        waiting_for_input = 1;
        while (waiting_for_input == 1) { }

        switch (last_press)
        {
        case KEY_ENTER:
            goto done;
        case KEY_BACKSPACE:
            if (index > 0)
            {
                buffer[index--] = '\0';
            }
            break;
        default:
            if (index < sizeof(buffer) - 1 && KBD_Key2Char(last_press) >= '0' && KBD_Key2Char(last_press) <= '9')
            {
                buffer[index++] = KBD_Key2Char(last_press);
                buffer[index] = '\0';
            }
            break;
        }

        // printf("\nBuffer: %s\n", buffer);
    }

done:
    *out = atoi(buffer);
    valid_input = 1;

    while (!valid_input)
    {
        printf("\nInvalid input, try again: ");
        KBD_ReadNumber(out);
    }
}
