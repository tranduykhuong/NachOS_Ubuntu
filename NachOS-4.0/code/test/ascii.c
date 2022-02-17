
#include "syscall.h"

int main()
{
    int i, temp;
    PrintStr("Bang ma ASCII:\n");
    for (i = 32; i <= 126; i++)
    {
        temp = i % 10;
        if (temp == 0 && temp != 32)
            PrintStr("\n");
        PrintChar((char)i);
        PrintChar(' ');
    }
    PrintStr("\n");
    Halt();
    /* not reached */
} 