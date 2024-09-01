#include "kbd.h"
#include <stdio.h>
#include "arch/i686/io.h"
#include "arch/i686/pic.h"
#include <string.h>
#include <memory.h>
#include <stdbool.h>

bool kbd_irq = false;
KBD_KEY last_press = NOOP;

bool caps_lock = false;
bool left_shift = false;
bool right_shift = false;
bool ctrl = false;

bool printing = false;
bool print_counter = false;

void KBD_IRQ(Registers* regs)
{
    last_press = i686_inb(0x60);
    uint8_t chr = KBD_Key2Char(last_press);

    
    if ((left_shift == true) || (right_shift == true) || (caps_lock == true)) {
        if (chr >= 'a' && chr <= 'z') {
            chr = chr - 'a' + 'A';
        }
    }
    
    if (last_press < 0x80)
    {
        kbd_irq = false;
    }

    switch (last_press)
    {
    case KEY_LEFT_SHIFT:
        left_shift = true;
        goto end;
    case KEY_RIGHT_SHIFT:
        right_shift = true;
        goto end;
    case KEY_LEFT_CONTROL:
        ctrl = true;
        goto end;
    case KEY_CAPS_LOCK:
        caps_lock = !caps_lock;
        goto end;
    case KEY_LEFT_SHIFT_RELEASED:
        left_shift = false;
        goto end;
    case KEY_RIGHT_SHIFT_RELEASED:
        right_shift = false;
        goto end;
    case KEY_LEFT_CONTROL_RELEASED:
        ctrl = false;
        goto end;
    }
    

    if (printing == true)
    {
        if (last_press == KEY_BACKSPACE)
        {
            if (print_counter > 0)
            {
                backspace();
                print_counter--;
            }
            
        }
        else if (chr != false)
        {
            printf("%c", chr);
            print_counter++;
        }
        
        
    }

end:
    i686_PIC_SendEndOfInterrupt(1);
    return;
}

void KBD_WaitForIRQ()
{
    kbd_irq = false;
    while (kbd_irq == false)
    return;
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
    printing = true;
    print_counter = 0;

    while (1)
    {
        // tell IRQ handler to notify on input
        KBD_WaitForIRQ();

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
        // printf("Index: %d\n", index);
    }

done:
    printing = false;
    strcpy(out, buffer);
}


void KBD_ReadNumber(uint32_t* out)
{
    static char buffer[256] = {0};
    int index = false;

    // reset buffer
    memset(buffer, 0, sizeof(buffer));

    // set up for printing
    printing = true;
    print_counter = false;

    while (1)
    {
        // tell IRQ handler to notify on input
        KBD_WaitForIRQ();

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
        // printf("\nIndex: %d\n", index);
    }

done:
    printing = false;
    int* error = false;
    *error = false;
    *out = atoi(buffer, error);
    if (*error == true)
    {
        printf("Invalid number: %s\n", buffer);
        printf("Try again: ");
        KBD_ReadNumber(out);
    }
    else
    {
       return;
    }
    
}
